# Extrapolation of image sequences #

OptFlow also implements some experimental algorithms for image extrapolation. The following methods are currently implemented:

**Inverse pixel-based extrapolation** (dense motion fields)
  * For each pixel in the second source image, find the corresponding pixel in the first source image. Requires an inverse motion field (image 2->image 1).
**Forward mesh extrapolation** (sparse motion fields)
  * Construct a textured triangle mesh from the origins of the motion vectors and transform the mesh by using a forward motion field (image 1->image 2).

Both of these approaches have limitations. As they extrapolate only existing pixels and don't take any intensity changes or generation of new pixels into account, they usually cause distortion or large "holes" in the image sequence.

**extrapolate** is the utility for extrapolating images with motion fields. To show its usage, run it without arguments:

```
$ ./extrapolate
Usage: extrapolate <required arguments>

General options:
  --help                print usage
  --version             print version number

Required arguments:
  --image arg           image to extrapolate
  --motionfield arg     motion field to use
  --numtimesteps arg    number of images to extrapolate
  --outprefix arg       output file prefix
```

Assuming that a motion field file named as "motion.psvm" has been generated with **extractmotion**, a sequence of 10 extrapolated images (named as test-extrapolated-`*`.png) can be generated as follows:

```
./extrapolate --image motion.psvm --numtimesteps 10 --outprefix test
```