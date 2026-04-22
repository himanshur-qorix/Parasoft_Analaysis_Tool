# Mka Module - Configuration 1

## Module Description

MKA (MACsec Key Agreement) module implements IEEE 802.1X-2010 key agreement protocol for secure communication. Handles authentication, key derivation, and secure channel establishment.

## Coding Standards

- **MISRA C 2012**: Mandatory rules enforced
- **CERT C**: Safety-critical guidelines
- **AUTOSAR C++14**: Automotive-specific rules
- **Custom**: Project-specific deviations documented in DR-2024-xxx series

## Common Justifications

### Frequently Justified Rules

| Rule | Reason | Approval |
|------|--------|----------|
| MISRA2012-RULE-11_5 | Legacy CAN stack API uses void pointers for message passing | DR-2024-045 |
| CERT-EXP36-C | Enum-to-int casts required for hardware register access | DR-2024-112 |
| MISRA2012-RULE-8_9 | Global state required for interrupt context safety | DR-2023-089 |
| AUTOSAR-M5-2-12 | Pointer arithmetic needed for DMA buffer management | DR-2024-201 |

### Typical Scenarios

1. **Hardware Register Access**
   - Direct memory access via volatile pointers
   - Justified for: MISRA 11.4, 11.5, 11.6
   - Safety: All addresses validated, memory mapped correctly

2. **Performance-Critical Paths**
   - Optimized loops for key derivation (crypto operations)
   - Justified for: MISRA 14.4 (non-boolean loop counter)
   - Validated through unit tests and timing analysis

3. **Legacy API Compatibility**
   - Interface with existing CAN stack v2.1
   - Justified for: Type casting rules
   - Migration planned for 2027 release

## Module Statistics

- **Lines of Code**: ~8,500
- **Files**: 12 (.c) + 8 (.h)
- **Parasoft Violations**: 156 total
  - Justified: 89 (57%)
  - Fixed: 67 (43%)
- **Quality Score**: 8.7/10

## Special Considerations

### Architecture Constraints

- **Real-Time**: 100μs response time requirement
- **Memory**: Limited 128KB RAM, stack optimization required
- **Safety**: ASIL-D rated, comprehensive justification needed
- **Platform**: ARM Cortex-M4, hardware-specific optimizations

### Design Decisions

1. **Global State Machine**: Required for interrupt safety, justified deviation from modular design
2. **Static Buffers**: No dynamic allocation due to safety requirements
3. **Inline Functions**: Performance-critical functions inlined (justified macro usage)

## Learning Value

### High-Quality Examples

This module provides excellent learning examples for:
- ✅ Proper hardware abstraction justifications
- ✅ Safety-critical deviation documentation
- ✅ Performance vs. compliance trade-offs
- ✅ Legacy API integration patterns

### What AI Can Learn

1. **Context Awareness**: Hardware constraints justify certain deviations
2. **Consistency**: Similar rules justified similarly across module
3. **Documentation**: DR references and detailed explanations
4. **Risk Assessment**: Safety implications clearly stated

## File Inventory

### Source Files
```
source/
├── Mka_Main.c          (1,200 LOC) - Main state machine
├── Mka_Crypto.c        (850 LOC)   - Key derivation
├── Mka_Protocol.c      (1,100 LOC) - Protocol handling
├── Mka_Hardware.c      (450 LOC)   - HW abstraction
├── Mka_Config.c        (300 LOC)   - Configuration
└── [8 header files]
```

### Reports
```
reports/
├── Mka_parasoft_full_report.xml          - Complete analysis
├── Mka_violations.json                   - Structured violations
├── Mka_suppress_comments_20260422.txt    - All justifications
└── Mka_quality_metrics.json              - Quality analysis
```

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.2 | 2026-04-15 | Updated justifications per new MISRA interpretation |
| 1.1 | 2026-02-10 | Added hardware abstraction layer |
| 1.0 | 2025-11-20 | Initial release with full Parasoft compliance |

## Contact

- **Module Owner**: MKA Development Team
- **Code Reviewer**: Senior Safety Engineer
- **Last Parasoft Scan**: April 22, 2026
- **Next Review**: July 2026

## Usage Notes for AI

### Pattern Recognition Focus

When AI analyzes this module, it should learn:

1. **Hardware-Related Justifications**
   ```
   Pattern: "Hardware register at address 0x..."
   → Usually justifies MISRA 11.x rules
   → Should reference memory map documentation
   ```

2. **Safety-Critical Context**
   ```
   Pattern: "ASIL-D requirement" + "tested via..."
   → More detailed justification expected
   → Should include testing references
   ```

3. **Performance Justifications**
   ```
   Pattern: "Real-time constraint" + timing analysis
   → Acceptable for loop optimizations
   → Must show measurements
   ```

4. **Legacy API Integration**
   ```
   Pattern: "Legacy CAN stack" + version reference
   → Standard justification template
   → Include migration timeline if available
   ```

### Quality Indicators

**Good Justifications in This Module:**
- Reference specific DR (Deviation Request) numbers
- Explain WHY, not just WHAT
- Include safety analysis where applicable
- Mention testing/validation approach

**Module-Specific Style:**
- Formal tone (safety-critical domain)
- Detailed technical explanations
- Regulatory compliance references
- Future migration plans where relevant

---

**This module is recommended for AI training due to high justification quality and comprehensive documentation.**

**Last Updated**: April 22, 2026  
**Parasoft Version**: 10.7.2  
**Configuration**: Automotive Safety Critical
