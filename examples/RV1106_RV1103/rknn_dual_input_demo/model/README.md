# Model Directory

Place your dual-input `.rknn` model file here.

The demo expects a model with:
- Input 0: image (64×64×3 uint8, NHWC)
- Input 1: features (16-dim, manually quantized to int8)
- Output: 8-class logits (int8)

See ../README.md for details on adjusting dimensions and quantization parameters.
