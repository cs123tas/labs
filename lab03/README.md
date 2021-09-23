# Lab 03 — Transformations & Scene Graphs

## Intro
In this Lab, you’ll learn what scene graphs are and how to parse them. You will implement a scene file parser, namely Scene::parse method, in your Intersect code. Ideally, you will be able to reuse Scene::parse in Ray and Sceneview without having to make any modifications.

## Understanding Scene Graphs
In order to visualize a complex 3-dimensional scene, thousands of tiny triangles must be drawn to the screen. It would be senseless to require the manual placement of each one of these triangles; instead, we usually define the scene in terms of the various primitives that compose it. Even better, we allow for primitives to be grouped, and then we reference those groups as user-defined primitives. Since such groupings can be nested, our representation of objects in a scene will form a directed acyclic graph, which we will call **scene graph**.

Throughout this course, we will use a CSCI1230-specific scene file format to describe a scene. Our scene file contains necessary information of a scene, namely global data, camera data, light data, and object data. You should refer to [this document](http://cs.brown.edu/courses/cs123/archive/2020/docs/scenefile.pdf) to understand scene files and scene graphs in detail.

Now, work on the following exercises!

#### *Task 1:*
Consider the following excerpt from a scene file:

![Task 1](img/0000.png)

Let C and C’ be the cones before and after applying transformations respectively, and let T, R, S be the translation, rotation, and scale matrices (respectively) corresponding to the transformations in the excerpt. How can you represent C’ with C, T, R, and S? Write your answer in the form of three matrices multiplied with C. Note that the order of multiplications is important since matrix multiplication is not commutative.

#### *Task 2:*
Consider the following excerpt from a scene file with nested transblocks:

![Task 2](img/0001.png)

Let M1 be the transformation matrix in the outer transblock (translation and scaling), and M2 be the transformation matrix in the inner trans block (rotation and scaling). In what order must you multiply these matrices to obtain a single composite matrix with the desired effect on the primitive sphere object?

#### *Task 3:*
Another way to represent a scene graph is to visualize it as a directed acyclic graph. Consider the following tree representation of a nested scene graph with 5 primitives, where transformation matrices for transblocks are named as M1, M2, and so on. The empty leaf node without any children represents a “tree” transblock with no sub-transblocks. For each primitive object, identify its transformation matrix, represented in the form of matrix multiplication.

![Task 3](img/0002.png)

## Efficient Parsing
After being sure of the order in which matrices must be multiplied, you should decide how to  efficiently traverse the primitive objects in a scene graph. 

#### #Task 4:*
In the previous exercise, you might have noticed that some matrix multiplications repeat throughout different objects. Explain why traversing a scene graph from the root node every time each object is rendered is inefficient and propose a more efficient approach. Explain how your approach is better in terms of time complexity.

You may get checked off for this lab at this point, but you **must** finish the remaining tasks for Intersect assignment.

## Implement a Parser
Now, you are ready to implement the 'static Scene::parse()' method in our project code.


