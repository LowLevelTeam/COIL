# Array/Vector Operations (0x90-0x9F)
Vector / Multiple Scalar Operations. Here are operations to act on multiple values as one.

## Instructions
#### VDOT (0x95)
Compute the dot product of vectors.

Operands:
- Destination: Non TYPE_VOID
- Left: TYPE_V* or TYPE_ARRAY
- Right: TYPE_V* or TYPE_ARRAY
- TYPE_PARAM5: branch_condition_t (optional)