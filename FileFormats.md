# File formats for storing vector fields #

OptFlow uses two different file formats for storing motion fields. The PDVM format is for dense motion fields, and the PSVM format is for storing sparse motion fields.

## Dense motion fields ##

The PDVM format for storing dense vector fields is a modification of the PGM format. It consists of a header and binary data. The text-format header is of the form:
```
PDV
[width] [height]
[numqualitychannels]
```
where numqualitychannels is the number of quality descriptors for each motion vector.

The binary-format data consists of 2+q -dimensional, where q is the number of quality channels, sequentially ordered vectors originating from each pixel in the raster. The first and second component represent the x- and y-components of a vector, respectively. The data is stored in 32-bit floating-point format.

## Sparse motion fields ##

Sparse motion fields can be more efficiently stored as lists of vectors, and the PSVM file format is specifically designed for this. A PSVM file consists of a header and data. The text-format header is of the form
```
PSV
[numvectors]
```
where numvectors is an integer representing the number of vectors stored in the file.

The data consists of sequentially ordered quartets
```
<x1,y1,x2,y2>
```
stored in binary format representing the start and end points of a vector. Each component is a 32-bit floating-point number.