/**
 * Parasoft CERT/MISRA Analyzer - VS Code Extension
 * Real-time violation detection and AI-powered fix suggestions
 */

const vscode = require('vscode');
const path = require('path');
const fs = require('fs');
const { spawn } = require('child_process');

// Global state
let diagnosticCollection;
let knowledgeBase = {};
let outputChannel;
let statusBarItem;

/**
 * Extension activation
 */
function activate(context) {
    console.log('Parasoft CERT/MISRA Analyzer is now active');
    
    // Create output channel
    outputChannel = vscode.window.createOutputChannel('Parasoft Analyzer');
    
    // Create diagnostic collection
    diagnosticCollection = vscode.languages.createDiagnosticCollection('parasoft');
    context.subscriptions.push(diagnosticCollection);
    
    // Create status bar item
    statusBarItem = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 100);
    statusBarItem.text = '$(bug) Parasoft';
    statusBarItem.tooltip = 'Parasoft CERT/MISRA Analyzer';
    statusBarItem.command = 'parasoft.analyzeCurrentFile';
    statusBarItem.show();
    context.subscriptions.push(statusBarItem);
    
    // Register commands
    context.subscriptions.push(
        vscode.commands.registerCommand('parasoft.analyzeCurrentFile', analyzeCurrentFile),
        vscode.commands.registerCommand('parasoft.analyzeWorkspace', analyzeWorkspace),
        vscode.commands.registerCommand('parasoft.clearDiagnostics', clearDiagnostics),
        vscode.commands.registerCommand('parasoft.suppressViolation', suppressViolation),
        vscode.commands.registerCommand('parasoft.generateFix', generateFix),
        vscode.commands.registerCommand('parasoft.openKnowledgeBase', openKnowledgeBase),
        vscode.commands.registerCommand('parasoft.openReport', openReport)
    );
    
    // Register code action provider for quick fixes
    context.subscriptions.push(
        vscode.languages.registerCodeActionsProvider(
            { language: 'c' },
            new ParasoftCodeActionProvider(),
            { providedCodeActionKinds: ParasoftCodeActionProvider.providedCodeActionKinds }
        )
    );
    
    context.subscriptions.push(
        vscode.languages.registerCodeActionsProvider(
            { language: 'cpp' },
            new ParasoftCodeActionProvider(),
            { providedCodeActionKinds: ParasoftCodeActionProvider.providedCodeActionKinds }
        )
    );
    
    // Watch for file changes
    const config = vscode.workspace.getConfiguration('parasoft');
    
    if (config.get('analyzeOnSave')) {
        context.subscriptions.push(
            vscode.workspace.onDidSaveTextDocument(document => {
                if (document.languageId === 'c' || document.languageId === 'cpp') {
                    analyzeDocument(document);
                }
            })
        );
    }
    
    if (config.get('analyzeOnType')) {
        let timeout = null;
        context.subscriptions.push(
            vscode.workspace.onDidChangeTextDocument(event => {
                const document = event.document;
                if (document.languageId === 'c' || document.languageId === 'cpp') {
                    clearTimeout(timeout);
                    timeout = setTimeout(() => analyzeDocument(document), 1500);
                }
            })
        );
    }
    
    // Load knowledge base
    loadKnowledgeBase();
    
    // Analyze currently open C/C++ files
    vscode.workspace.textDocuments.forEach(document => {
        if (document.languageId === 'c' || document.languageId === 'cpp') {
            analyzeDocument(document);
        }
    });
    
    outputChannel.appendLine('✓ Parasoft CERT/MISRA Analyzer activated');
    outputChannel.appendLine(`✓ Workspace: ${vscode.workspace.workspaceFolders?.[0]?.uri.fsPath || 'No workspace'}`);
}

/**
 * Load knowledge base from JSON file
 */
function loadKnowledgeBase() {
    try {
        const config = vscode.workspace.getConfiguration('parasoft');
        const moduleName = config.get('moduleName') || 'Mka';
        
        const toolPath = findToolPath();
        if (!toolPath) {
            outputChannel.appendLine('⚠️ Parasoft tool directory not found');
            return;
        }
        
        const kbPath = path.join(toolPath, 'knowledge_base', `${moduleName}_KnowledgeDatabase.json`);
        
        if (fs.existsSync(kbPath)) {
            const data = fs.readFileSync(kbPath, 'utf8');
            knowledgeBase = JSON.parse(data);
            outputChannel.appendLine(`✓ Loaded Knowledge Base: ${kbPath}`);
            outputChannel.appendLine(`  Total violations: ${Object.keys(knowledgeBase.violations || {}).length}`);
        } else {
            outputChannel.appendLine(`⚠️ Knowledge Base not found: ${kbPath}`);
        }
    } catch (error) {
        outputChannel.appendLine(`❌ Error loading Knowledge Base: ${error.message}`);
    }
}

/**
 * Find Parasoft tool path
 */
function findToolPath() {
    const config = vscode.workspace.getConfiguration('parasoft');
    let toolPath = config.get('toolPath');
    
    if (toolPath && fs.existsSync(toolPath)) {
        return toolPath;
    }
    
    // Auto-detect: check if we're in the tool directory
    const workspaceFolder = vscode.workspace.workspaceFolders?.[0]?.uri.fsPath;
    if (workspaceFolder) {
        // Check if current workspace IS the tool
        if (fs.existsSync(path.join(workspaceFolder, 'src', 'parasoft_gui.py'))) {
            return workspaceFolder;
        }
        
        // Check parent directory
        const parentDir = path.dirname(workspaceFolder);
        if (fs.existsSync(path.join(parentDir, 'src', 'parasoft_gui.py'))) {
            return parentDir;
        }
    }
    
    return null;
}

/**
 * Analyze current file
 */
async function analyzeCurrentFile() {
    const editor = vscode.window.activeTextEditor;
    if (!editor) {
        vscode.window.showWarningMessage('No active editor');
        return;
    }
    
    const document = editor.document;
    if (document.languageId !== 'c' && document.languageId !== 'cpp') {
        vscode.window.showWarningMessage('Please open a C/C++ file');
        return;
    }
    
    await analyzeDocument(document);
}

/**
 * Analyze entire workspace
 */
async function analyzeWorkspace() {
    const config = vscode.workspace.getConfiguration('parasoft');
    if (!config.get('enabled')) {
        vscode.window.showWarningMessage('Parasoft analyzer is disabled');
        return;
    }
    
    vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: 'Analyzing workspace...',
        cancellable: true
    }, async (progress, token) => {
        const files = await vscode.workspace.findFiles('**/*.{c,cpp,h,hpp}', '**/node_modules/**');
        
        for (let i = 0; i < files.length; i++) {
            if (token.isCancellationRequested) {
                break;
            }
            
            progress.report({
                message: `${i + 1}/${files.length} files`,
                increment: (100 / files.length)
            });
            
            const document = await vscode.workspace.openTextDocument(files[i]);
            await analyzeDocument(document, false);
        }
        
        vscode.window.showInformationMessage(`✓ Analyzed ${files.length} files`);
    });
}

/**
 * Analyze a document and create diagnostics
 */
async function analyzeDocument(document, showMessage = true) {
    const config = vscode.workspace.getConfiguration('parasoft');
    if (!config.get('enabled')) {
        return;
    }
    
    const filePath = document.uri.fsPath;
    const fileName = path.basename(filePath);
    
    outputChannel.appendLine(`\n[${new Date().toLocaleTimeString()}] Analyzing: ${fileName}`);
    statusBarItem.text = '$(sync~spin) Analyzing...';
    
    try {
        // Find violations for this file in knowledge base
        const violations = findViolationsForFile(fileName);
        
        // Create diagnostics
        const diagnostics = violations.map(v => createDiagnostic(v, document));
        
        // Update diagnostics collection
        diagnosticCollection.set(document.uri, diagnostics);
        
        // Update status bar
        statusBarItem.text = `$(bug) ${diagnostics.length} violation${diagnostics.length !== 1 ? 's' : ''}`;
        
        outputChannel.appendLine(`  Found ${diagnostics.length} violations`);
        
        if (showMessage && diagnostics.length > 0) {
            const certCount = diagnostics.filter(d => d.code?.toString().startsWith('CERT')).length;
            const misraCount = diagnostics.filter(d => d.code?.toString().startsWith('MISRA')).length;
            vscode.window.showInformationMessage(
                `Found ${diagnostics.length} violations (CERT: ${certCount}, MISRA: ${misraCount})`
            );
        }
    } catch (error) {
        outputChannel.appendLine(`  ❌ Error: ${error.message}`);
        statusBarItem.text = '$(bug) Error';
    }
}

/**
 * Find violations for a specific file from knowledge base
 */
function findViolationsForFile(fileName) {
    const config = vscode.workspace.getConfiguration('parasoft');
    const severityFilter = config.get('severityFilter') || ['CRITICAL', 'HIGH', 'MEDIUM'];
    const showCERT = config.get('showCERT');
    const showMISRA = config.get('showMISRA');
    
    const violations = [];
    const kbViolations = knowledgeBase.violations || {};
    
    for (const [id, violation] of Object.entries(kbViolations)) {
        // Check severity filter
        const severity = violation.severity || 'MEDIUM';
        if (!severityFilter.includes(severity)) {
            continue;
        }
        
        // Check CERT/MISRA filter
        const violationId = violation.violation_id || '';
        if (violationId.startsWith('CERT') && !showCERT) continue;
        if (violationId.startsWith('MISRA') && !showMISRA) continue;
        
        // Check if violation affects this file
        const filesAffected = violation.files_affected || [];
        for (const fileEntry of filesAffected) {
            let file = '';
            let line = 1;
            
            if (typeof fileEntry === 'object') {
                file = fileEntry.file || '';
                line = fileEntry.line || 1;
            } else {
                file = String(fileEntry);
            }
            
            // Match file name (case-insensitive)
            if (file.toLowerCase().includes(fileName.toLowerCase()) || 
                fileName.toLowerCase().includes(path.basename(file).toLowerCase())) {
                violations.push({
                    ...violation,
                    file,
                    line,
                    id
                });
            }
        }
    }
    
    return violations;
}

/**
 * Create a diagnostic from a violation
 */
function createDiagnostic(violation, document) {
    const line = Math.max(0, (violation.line || 1) - 1);
    const lineText = document.lineAt(Math.min(line, document.lineCount - 1)).text;
    const range = new vscode.Range(line, 0, line, lineText.length);
    
    const severity = getSeverity(violation.severity);
    const message = violation.violation_text || 'Unknown violation';
    const code = violation.violation_id || 'UNKNOWN';
    
    const diagnostic = new vscode.Diagnostic(range, message, severity);
    diagnostic.code = code;
    diagnostic.source = 'Parasoft';
    
    // Add related information
    if (violation.fix_suggestion) {
        diagnostic.relatedInformation = [
            new vscode.DiagnosticRelatedInformation(
                new vscode.Location(document.uri, range),
                `Fix suggestion available: ${violation.fix_suggestion.description || 'Generate fix'}`
            )
        ];
    }
    
    return diagnostic;
}

/**
 * Convert severity string to VS Code diagnostic severity
 */
function getSeverity(severityStr) {
    switch ((severityStr || '').toUpperCase()) {
        case 'CRITICAL':
        case 'HIGH':
            return vscode.DiagnosticSeverity.Error;
        case 'MEDIUM':
            return vscode.DiagnosticSeverity.Warning;
        case 'LOW':
            return vscode.DiagnosticSeverity.Information;
        default:
            return vscode.DiagnosticSeverity.Warning;
    }
}

/**
 * Clear all diagnostics
 */
function clearDiagnostics() {
    diagnosticCollection.clear();
    statusBarItem.text = '$(bug) Parasoft';
    vscode.window.showInformationMessage('Cleared all violations');
}

/**
 * Suppress a violation at cursor
 */
async function suppressViolation() {
    const editor = vscode.window.activeTextEditor;
    if (!editor) return;
    
    const position = editor.selection.active;
    const diagnostics = diagnosticCollection.get(editor.document.uri) || [];
    
    // Find diagnostic at cursor position
    const diagnostic = diagnostics.find(d => d.range.contains(position));
    if (!diagnostic) {
        vscode.window.showWarningMessage('No violation found at cursor position');
        return;
    }
    
    // Ask for justification
    const justification = await vscode.window.showInputBox({
        prompt: 'Enter justification for suppression',
        placeHolder: 'e.g., False positive - variable is initialized in external module'
    });
    
    if (!justification) return;
    
    // Insert suppression comment
    const line = diagnostic.range.start.line;
    const indent = editor.document.lineAt(line).text.match(/^\s*/)?.[0] || '';
    const suppressComment = `${indent}/* parasoft-suppress ${diagnostic.code} "${justification}" */\n`;
    
    await editor.edit(editBuilder => {
        editBuilder.insert(new vscode.Position(line, 0), suppressComment);
    });
    
    // Save document
    await editor.document.save();
    
    vscode.window.showInformationMessage(`✓ Suppressed ${diagnostic.code}`);
    
    // Re-analyze
    analyzeDocument(editor.document, false);
}

/**
 * Generate AI fix for violation
 */
async function generateFix() {
    const editor = vscode.window.activeTextEditor;
    if (!editor) return;
    
    const position = editor.selection.active;
    const diagnostics = diagnosticCollection.get(editor.document.uri) || [];
    
    // Find diagnostic at cursor position
    const diagnostic = diagnostics.find(d => d.range.contains(position));
    if (!diagnostic) {
        vscode.window.showWarningMessage('No violation found at cursor position');
        return;
    }
    
    vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: `Generating fix for ${diagnostic.code}...`,
        cancellable: false
    }, async () => {
        try {
            const fix = await callPythonFixGenerator(diagnostic, editor.document);
            
            if (fix && fix.code_suggestion) {
                const action = await vscode.window.showInformationMessage(
                    `Fix generated: ${fix.description}`,
                    'Apply Fix',
                    'Show Details',
                    'Cancel'
                );
                
                if (action === 'Apply Fix') {
                    await applyFix(editor, diagnostic, fix);
                } else if (action === 'Show Details') {
                    showFixDetails(fix);
                }
            } else {
                vscode.window.showWarningMessage('No fix could be generated');
            }
        } catch (error) {
            vscode.window.showErrorMessage(`Error generating fix: ${error.message}`);
        }
    });
}

/**
 * Call Python fix generator
 */
function callPythonFixGenerator(diagnostic, document) {
    return new Promise((resolve, reject) => {
        const config = vscode.workspace.getConfiguration('parasoft');
        const toolPath = findToolPath();
        const pythonPath = config.get('pythonPath') || 'python';
        const moduleName = config.get('moduleName') || 'Mka';
        
        if (!toolPath) {
            reject(new Error('Parasoft tool path not found'));
            return;
        }
        
        const scriptPath = path.join(toolPath, 'src', 'generate_fix_single.py');
        
        // Create temporary script if it doesn't exist
        if (!fs.existsSync(scriptPath)) {
            const scriptContent = `
import sys
import json
from pathlib import Path
sys.path.insert(0, str(Path(__file__).parent))
from CodeFixGenerator import CodeFixGenerator
from KnowledgeDatabaseManager import KnowledgeDatabaseManager

violation_id = sys.argv[1]
module_name = sys.argv[2]

kb_manager = KnowledgeDatabaseManager(module_name)
fix_gen = CodeFixGenerator(module_name, kb_manager, Path('fixes'), {})

violations = kb_manager.knowledge_base.get('violations', {})
if violation_id in violations:
    fix = fix_gen._generate_fix_for_violation(violations[violation_id])
    print(json.dumps(fix, indent=2))
else:
    print(json.dumps({'error': 'Violation not found'}))
`;
            fs.writeFileSync(scriptPath, scriptContent);
        }
        
        const args = [scriptPath, diagnostic.code, moduleName];
        const proc = spawn(pythonPath, args, { cwd: toolPath });
        
        let output = '';
        let error = '';
        
        proc.stdout.on('data', data => output += data.toString());
        proc.stderr.on('data', data => error += data.toString());
        
        proc.on('close', code => {
            if (code === 0 && output) {
                try {
                    const fix = JSON.parse(output);
                    resolve(fix);
                } catch (e) {
                    reject(new Error(`Failed to parse fix: ${e.message}`));
                }
            } else {
                reject(new Error(error || 'Fix generation failed'));
            }
        });
    });
}

/**
 * Apply fix to document
 */
async function applyFix(editor, diagnostic, fix) {
    const line = diagnostic.range.start.line;
    const lineText = editor.document.lineAt(line).text;
    const indent = lineText.match(/^\s*/)?.[0] || '';
    
    // Apply code suggestion
    const newCode = indent + fix.code_suggestion.example;
    
    await editor.edit(editBuilder => {
        editBuilder.replace(diagnostic.range, newCode);
    });
    
    await editor.document.save();
    vscode.window.showInformationMessage('✓ Fix applied');
    
    // Re-analyze
    analyzeDocument(editor.document, false);
}

/**
 * Show fix details in new editor
 */
function showFixDetails(fix) {
    const content = `# Fix Details

## Description
${fix.description || 'No description'}

## Priority
${fix.priority || 'MEDIUM'}

## Code Suggestion
\`\`\`c
${fix.code_suggestion?.example || 'No code example'}
\`\`\`

## Explanation
${fix.code_suggestion?.explanation || 'No explanation'}

## References
${fix.references?.join('\n') || 'No references'}
`;
    
    vscode.workspace.openTextDocument({ content, language: 'markdown' }).then(doc => {
        vscode.window.showTextDocument(doc, { preview: true });
    });
}

/**
 * Open knowledge base in editor
 */
function openKnowledgeBase() {
    const config = vscode.workspace.getConfiguration('parasoft');
    const moduleName = config.get('moduleName') || 'Mka';
    const toolPath = findToolPath();
    
    if (!toolPath) {
        vscode.window.showWarningMessage('Parasoft tool path not found');
        return;
    }
    
    const kbPath = path.join(toolPath, 'knowledge_base', `${moduleName}_KnowledgeDatabase.json`);
    
    if (fs.existsSync(kbPath)) {
        vscode.workspace.openTextDocument(kbPath).then(doc => {
            vscode.window.showTextDocument(doc);
        });
    } else {
        vscode.window.showErrorMessage(`Knowledge Base not found: ${kbPath}`);
    }
}

/**
 * Open CERT/MISRA report
 */
function openReport() {
    const config = vscode.workspace.getConfiguration('parasoft');
    const moduleName = config.get('moduleName') || 'Mka';
    const toolPath = findToolPath();
    
    if (!toolPath) {
        vscode.window.showWarningMessage('Parasoft tool path not found');
        return;
    }
    
    const reportPath = path.join(toolPath, 'reports', `${moduleName}_CERT_MISRA_Report.html`);
    
    if (fs.existsSync(reportPath)) {
        vscode.env.openExternal(vscode.Uri.file(reportPath));
    } else {
        vscode.window.showErrorMessage(`Report not found: ${reportPath}`);
    }
}

/**
 * Code Action Provider for quick fixes
 */
class ParasoftCodeActionProvider {
    static providedCodeActionKinds = [
        vscode.CodeActionKind.QuickFix
    ];
    
    provideCodeActions(document, range, context, token) {
        const actions = [];
        
        for (const diagnostic of context.diagnostics) {
            if (diagnostic.source === 'Parasoft') {
                // Suppress action
                const suppressAction = new vscode.CodeAction(
                    `Suppress ${diagnostic.code}`,
                    vscode.CodeActionKind.QuickFix
                );
                suppressAction.command = {
                    command: 'parasoft.suppressViolation',
                    title: 'Suppress violation'
                };
                suppressAction.diagnostics = [diagnostic];
                actions.push(suppressAction);
                
                // Generate fix action
                const fixAction = new vscode.CodeAction(
                    `Generate AI fix for ${diagnostic.code}`,
                    vscode.CodeActionKind.QuickFix
                );
                fixAction.command = {
                    command: 'parasoft.generateFix',
                    title: 'Generate fix'
                };
                fixAction.diagnostics = [diagnostic];
                fixAction.isPreferred = true;
                actions.push(fixAction);
            }
        }
        
        return actions;
    }
}

/**
 * Deactivation
 */
function deactivate() {
    if (diagnosticCollection) {
        diagnosticCollection.dispose();
    }
    if (outputChannel) {
        outputChannel.dispose();
    }
    if (statusBarItem) {
        statusBarItem.dispose();
    }
}

module.exports = {
    activate,
    deactivate
};
