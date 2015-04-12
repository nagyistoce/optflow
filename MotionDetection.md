# Implemented motion extraction algorithms #

OptFlow implements the following algorithms for motion detection:
  * Lucas & Kanade local least-squares algorithm
  * Proesmans et. al. global anisotropic diffusion algorithm
  * Lucas & Kanade algorithm (OpenCV implementation)

The first two algorithms are implemented by the author.
They generate dense vector fields, i.e. motion vectors for
each pixel in the source image. On the other hand, the OpenCV
implementation generates sparse motion fields. It extracts
the motion of a given set of "feature points". All implemented algorithms
use image pyramids in order to enhance the range of the detected motion.

**NOTE:** The current version supports only 8-bit grayscale images. Due to the limitations of the image pyramid technique, the width and height of the source images must also be powers of two.

# Running the algorithms #

**extractmotion** is the utility for running different motion extraction algorithms. To show the list of available command-line options, run it with no arguments:

```
$ ./extractmotion
Usage: extractmotion <required arguments> [algorithm-specific options]:

General options:
  --help                print usage
  --options arg         print options specific to the algorithm <arg>
  --version             print version number

Required arguments:
  --image1 arg          first image
  --image2 arg          second image
  --algorithm arg       motion detection algorithm (lucaskanade, opencv,
                        proesmans)
  --outprefix arg       output file prefix

```

An example image sequence representing moving objects with translation and rotation is provided with the package.

<img src='http://optflow.googlecode.com/svn/wiki/images/simple1.png' width='256' height='256'>
<img src='http://optflow.googlecode.com/svn/wiki/images/simple2.png' width='256' height='256'>

The following example runs the Proesmans algorithm with its default settings for this image sequence:<br>
<pre><code>./extractmotion --image1 simple1.png --image2 simple2.png --algorithm proesmans --outprefix test-simple<br>
</code></pre>

The following output files are produced:<br>
<ul><li>test-simple-motion.png<br>
</li><li>test-simple-motion.pdvm<br>
</li><li>test-simple-quality1.png<br>
representing the motion vector image, the computed motion field and a motion vector quality image, respectively. For specification of the pdvm format, see the FileFormats wiki page.</li></ul>

<img src='http://optflow.googlecode.com/svn/wiki/images/proesmans_simple_motion.png' width='256' height='256'>
<img src='http://optflow.googlecode.com/svn/wiki/images/proesmans_simple_quality1.png' width='256' height='256'>

<h1>References</h1>

The motion detection algorithms in this package are based on the<br>
following articles:<br>
<br>
<pre><code>[1] J. Bouguet, Pyramidal Implementation of the Lucas Kanade <br>
    Feature Tracker: Description of the Algorithm, Technical <br>
    report, OpenCV documents, Intel Corporation, Microprocessor <br>
    Research Labs, 2000<br>
<br>
[2] B.D. Lucas and T. Kanade, An iterative image registration <br>
    technique with an application to stereo vision, in Proc. <br>
    Seventh International Joint Conference on Artificial <br>
    Intelligence, Vancouver, 1981, pp. 674-679<br>
<br>
[3] M. Proesmans, L. Van Gool, E. Pauwels, and A. Oosterlinck, <br>
    Determination of optical ﬂow and its discontinuities using <br>
    non-linear diffusion, in 3rd European Conference on Computer <br>
    Vision, ECCV’94, 1994, Vol. 2, pp. 295–304.<br>
</code></pre>

and the C-based implementations found at:<br>
<ul><li><a href='ftp://ftp.csd.uwo.ca/pub/vision'>ftp://ftp.csd.uwo.ca/pub/vision</a>
</li><li><a href='http://of-eval.sourceforge.net'>http://of-eval.sourceforge.net</a>