# point-cloud-cylinder-fitting
The repository contains code for fitting some three dimensional point cloud data inside a cylinder.

## Problem description
The task is to read a file with points from the a .ply file to find the parameters of a cylinder model that fits it best.

A minimal description of that cylinder model would be a point and axis for the center of the cylinder and a radius.

## Solution approach
At first the points are wrapped up inside a 3D box. The steps to do that are given below:

```
1. Find the point with maximum x, y, z
2. Find the point with minimum x, y, z
3. Draw a 3D box where 1 and 2 are two diagonal points and other corner points are computed from them
```

After that, a cylinder wrapped the 3D box from outside. That way all points get fitted inside the cylinder. The steps to do that are given below:

```
1. Find a face of the 3D box to wrap inside a circle (i.e. circle will be the cylinder face)
2. Take the largest diagonal as diameter and diagonals' intersection point as center.
3. Find center of the cylinder.
```
	
## Steps to run
The repository contains a **cmesh0.ply** file which is the input. The **Program.cpp** is the code file. Run the .cpp file to get the wrapping cylinder information.

## Language used
C++

## IDE used
Codeblocks