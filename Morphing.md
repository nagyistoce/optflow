# Morphing between two images #

The third feature implemented in OptFlow is morphing between two
images. In addition to cross-fading between images, the algorithm
uses the forward and inverse motion fields to warp the images
closer to each other before interpolating pixel values linearly.
This technique yields much better results than simple cross-fading.

To be written...