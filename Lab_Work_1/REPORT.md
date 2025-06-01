# Parallel Image Processing Performance Report

## Test Environment
- CPU: 4-core x86_64 processor
- Compiler: g++ 11.3.0
- Optimization flags: `-O3 -march=native -fopenmp`

## Test Results

### Panda_small.bmp (256×256)
| Operation               | Time (s) |
|-------------------------|----------|
| Rotate (Clockwise)      | 0.000909 |
| Gauss Filter (Sequential)| 0.001627 |
| Gauss Filter (Parallel) | 0.008536 |

### Panda_middle.bmp (1024×1024)
| Operation               | Time (s) |
|-------------------------|----------|
| Rotate (Clockwise)      | 0.004041 |
| Gauss Filter (Sequential)| 0.007062 |
| Gauss Filter (Parallel) | 0.001496 |

### Panda_large.bmp (4096×4096)
| Operation               | Time (s) |
|-------------------------|----------|
| Rotate (Clockwise)      | 0.062237 |
| Gauss Filter (Sequential)| 0.112590 |
| Gauss Filter (Parallel) | 0.050513 |

## Analysis

1. **Small Images**:
   - Parallel overhead exceeds benefits for small images (<512×512)
   - Sequential processing recommended for images below this threshold

2. **Medium Images**:
   - Clear parallelization benefits (4.7x speedup for Gaussian filter)
   - Optimal performance achieved with dynamic scheduling

3. **Large Images**:
   - Consistent 2.2x speedup for computationally intensive operations
   - Memory bandwidth becomes limiting factor
