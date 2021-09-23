# Lab 03 — Transformations & Scene Graphs

## Intro
In this Lab, you’ll learn what scene graphs are and how to parse them. You will implement a scene file parser, namely Scene::parse method, in your Intersect code. Ideally, you will be able to reuse Scene::parse in Ray and Sceneview without having to make any modifications.

## Understanding Scene Graphs
In order to visualize a complex 3-dimensional scene, thousands of tiny triangles must be drawn to the screen. It would be senseless to require the manual placement of each one of these triangles; instead, we usually define the scene in terms of the various primitives that compose it. Even better, we allow for primitives to be grouped, and then we reference those groups as user-defined primitives. Since such groupings can be nested, our representation of objects in a scene will form a directed acyclic graph, which we will call **scene graph**.

Throughout this course, we will use a CSCI1230-specific scene file format to describe a scene. Our scene file contains necessary information of a scene, namely global data, camera data, light data, and object data. You should refer to [this document](http://cs.brown.edu/courses/cs123/archive/2020/docs/scenefile.pdf) to understand scene files and scene graphs in detail.

