

# Parasoft_Analysis_Tool


Version: 1.0.0
Developer: Himanshu R
Platform: Windows
Language: Python

## Dependencies

- beautifulsoup4
- lxml
- pandas

Install dependencies:
```
pip install beautifulsoup4 lxml pandas
```

## Features

- Parses Parasoft HTML reports for static analysis violations
- Retrieval-Augmented Generation (RAG) workflow:
	- Retrieves and suggests fixes from a shared knowledge base (fix_knowledge_base.json)
	- Applies fixes automatically (if enabled)
	- Logs new fixes for future learning and sharing

## Usage

1. Place your Parasoft HTML report (e.g., `report_dev1.html`) in the tool folder.
2. Run the tool from the command line:
	 ```
	 python ParasoftAnalysisTool.py <parasoft_report.html> [--apply]
	 ```
	 - Use `--apply` to automatically apply and log fixes where available.
3. The tool will suggest or apply fixes based on the knowledge base.
4. If a violation has no known fix, fix it manually and update the knowledge base for future runs.

## Knowledge Base

- All fixes are stored in `fix_knowledge_base.json` in the repository.
- The tool learns from every fix applied or logged, improving suggestions for future violations.

## Collaboration

- Anyone can pull the repository, provide access to their module/report, and benefit from shared fixes.
- Contribute by updating the knowledge base with new fixes to help others.

## Batch Usage

You can also use `Run.bat` for convenience, or integrate the script into your CI/CD pipeline.
