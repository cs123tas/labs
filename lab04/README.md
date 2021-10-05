# Lab 4: Maya
## Introduction
In this lab, we will learn how to create geometry, assign materials, set up lights and cameras, and render images in Autodesk Maya. This lab will not involve coding. We are presenting you with this knowledge so you can create content to use in your own graphics code, or to render content created from your graphics code. This lab looks long but don’t worry. We just added many images because the Maya interface can be confusing at first, and there are multiple optional components. Our hope is that the optional components might spark interest and/or come in handy for your final projects. That said, start early because installation and rendering can take time.
### Check Off
This lab contains many optional parts. To be checked off, you need
- 2 geometries
- 1 imported geometry
- 2 different materials
- 1 fully rendered image
- 1 exported obj file
## Installing Maya
Go to https://www.autodesk.com/education/free-software/maya, create an educational account using your brown.edu email address and install Maya. This tutorial uses Maya 2022. Make sure “Arnold renderer for Maya 2022” and “PyMel for Python 3” are checked.

After you are done installing, click Start, and enter your Maya credentials if prompted. You should now see an empty scene, or a screen that allows you to create a new file (which you should do), and then an empty scene. 

![installing maya](/lab04/images/install.png)

## Setting up a project
Before we start, we will save the file. In Maya, file paths are very important. Create a new directory on your computer and save the scene: **File ➜ Save scene as ➜** *select directory ➜ enter file name* **➜ Files of type: Maya Binary ➜ save as**. 

Next, set your project: **File ➜ Set project ➜** *select directory* **➜ Set ➜ Create default workspace**. This will generate a workspace.mel file and create a directory structure of the folders associated with your scene. 

Maya crashes often. So make sure to save your work periodically throughout the lab.

If you are not using a three-button mouse, (i.e. MacBook trackpad) change the Mouse Tracking to Two Buttons: **Windows ➜ Setting/Preferences ➜ Preferences ➜ Interface ➜ Devices ➜ Mouse tracking ➜ Two Buttons ➜ Save**.

## Navigating the window
The following functions will be useful when manipulating the camera view:

![manipulating camera view](/lab04/images/camera.png)

## Creating Geometry
Let’s create a sphere: **Poly Modeling ➜ Sphere Icon**. A sphere should appear in your workspace.

![sphere icon](/lab04/images/sphere.png)

With the sphere selected, click on the manipulators to move, rotate, and scale the sphere. Alternatively, you can enter values in the channel box. If you do not see the channel box on your screen, you need to select its icon (highlighted in blue in the upper right hand corner in the following image). 

![manipulating sphere](/lab04/images/spherewtransform.png)

![how to transform objects](/lab04/images/transform.png)

Let’s add a cylinder. 

![cylinder added](/lab04/images/addcylinder.png)

## Optional: Smooth Mesh
If you zoom in you will notice that the sphere and the cylinder are faceted. This is because the Smooth Mesh Preview is turned off. To turn it on: Shift select the sphere and the cylinder ➜ hit 3.

![smoothing mesh hotkeys](/lab04/images/smoothmesh.png)

![cylinder w smoothing](/lab04/images/smoothcylinder.png)

We see that something funny happens with the cylinder. This is because Maya uses the Catmull-Clark algorithm to subdivide the surfaces. So when there are not enough vertices to sample from, we get funny looking shapes. We will fix this by adding edge loops to hold the shape. Go back to regular viewing mode by hitting 1. Select the edge loop tool: **Mesh Tools ➜ Insert Edge Loops**. Click on one of the edges on the side of the cylinder and drag the loop close to the top base. Repeat this for the bottom base. To exit the edge loop tool, hit “q” or click on the arrow icon.

![cylinder w edge loop](/lab04/images/cylinderedgeloop.png)

Insert Edge Loops will only work for quadrilaterals. To add edges at the base of the cylinder, we will use the Extrude tool. Right click the cylinder to bring up the quick change menu. Then still holding right click down, drag your cursor over “Face”, and release.

![selecting face for extrusion](/lab04/images/face.png)

Shift select all the faces on the top base (make sure you don’t have other faces selected). Then, extrude: **Edit Mesh ➜ Extrude ➜ ** *change* **Offset** *to 0.2*. Repeat this for the bottom base. Preview your cylinder in Smooth Mesh Preview and it should now look more like a cylinder. 

![better-looking cylinder](/lab04/images/bettercylinder.png)

## Optional: Modeling
Feel free to add more polygons to your scene. If you are looking for a challenge, try modeling an object. [Here](https://www.youtube.com/watch?v=PntVhzpWDKY)’s a tutorial for a coffee mug.

## Importing/Exporting Geometry
One important thing to know is how to import and export geometries. That way, you can render objects you created in your code in Maya, and vice versa. Maya supports several file types, but we will focus on obj files. Let’s import a cow: **File ➜ Import ➜** *select cow.obj* ** ➜ Import**. 

![cow imported into scene](/lab04/images/cowimport.png)

Now let’s export the cylinder: *select cylinder* **➜ File ➜ Export Selection ➜ ** * choose location ➜ enter filename * **➜ Files of type: OBJexport ➜ Export**. This will generate a cylinder.obj file and a cylinder.mtl file. The mtl file describes how a program should apply textures to that object. 

If you want to add more objects, you can search for [free obj files online](https://www.cgtrader.com/items/2239831/download-page).

## Arnold Rendering
Let’s render what we have so far. Arnold is a Monte Carlo ray tracing renderer available as a plug-in for Maya. Open the Arnold Tab and click on the icon with an eye to open Arnold Render View. Click the red button to start. We see that the progress bar has reached 100% but the image is completely black. This is because we don’t have any lights. 

![dark scene](/lab04/images/darkscene.png)

We have provided you with a stage.mb file which includes three area lights, a backdrop and a render camera. Go ahead and import the file: **File ➜ Import ➜** * select stage.mb * ** ➜ Import**.

![import stage](/lab04/images/stage.png)

To view our scene from the render camera: **Panels ➜ Perspective ➜ stage: render_cam**.

![some objects out of frame](/lab04/images/outofframe.png)

We noticed that some of our objects are out of frame. To transform multiple objects at once, we can group the objects and apply hierarchical transformation. To do this, open the outliner: **Windows ➜ Outliner**. All objects from the stage.mb file have already been grouped under “group”. Create a new group containing only our objects: *Shift select objects (cylinder, sphere, cow, Wall-E) in the outline* **  ➜ Edit  ➜ Group**. 

![grouping objects](/lab04/images/group.png)

Now, Select the newly created **group1**, and apply transformations.

![transform the group](/lab04/images/grouptransform.png)

Let’s try rendering our scene again. Make sure the camera is set to **stage:render_cam**, and hit start.

![first successful arnold render](/lab04/images/nicerender1.png)

Much better! Go ahead and save the image: **File (in Arnold Render View) ➜ Save Image ➜ ** *enter name* ** ➜ Save**. Make sure to stop or close the Arnold Render View as it will slow down your computer.

## Optional: Lights and Cameras
We will go over some ways to modify your stage. Here are some more guides on [lights](https://docs.arnoldrenderer.com/display/A5AFMUG/Lights) and [cameras](https://docs.arnoldrenderer.com/display/A5AFMUG/Cameras).

To change the color of the lights:  *select* ** left_lightShape**  *in the outliner* ** ➜Attribute Editor ➜ stage:left_lightShape ➜ Arnold Area Light Attribute ➜ **  *pick a color*.

To change the brightness of the lights: *select* **left_lightShape** *in the outliner* **➜Attribute Editor ➜ stage:left_lightShape ➜ Arnold Area Light Attribute ➜ adjust exposure**.

![where the light settings are](/lab04/images/lightsettings.png)
![high exposure](/lab04/images/highexposure.png)
![low exposure](/lab04/images/lowexposure.png)

To change the Focal Length and Angle of View of the camera: *select* **stage:render_camShape** *in the outliner* **➜Attribute Editor ➜stage:render_camShape ➜ Camera Attribute ➜ adjust Angle of View and Focal Length**.

![camera parameters adjusted](/lab04/images/cameraadjust.png)


As with other objects in the scene, you can transform your lights and cameras using the move, scale, and rotate tools. For cameras, just make sure that the Lock Camera is off. 

## Assigning Materials

Let’s assign materials to objects: right click on the object **➜ Assign New Material ➜ Arnold ➜ Shader ➜ Surface ➜ AiStandardSurface**. Rename this to “sphereShader”. Try changing the base color of the sphere: **Attribute Editor ➜ sphereShader ➜ Base ➜** *pick a color*. Now render the scene. Try playing around with other attributes! ([base](https://docs.arnoldrenderer.com/display/A5AFMUG/Base), [specular](https://docs.arnoldrenderer.com/display/A5AFMUG/Specular), [transmission](https://docs.arnoldrenderer.com/display/A5AFMUG/Transmission) 

![assigning materials to sphere](/lab04/images/assignmaterials1.png)
![assigning materials to sphere](/lab04/images/assignmaterials2.png)
![assigning materials to sphere](/lab04/images/assignmaterials3.png)
![rendered orange sphere](/lab04/images/orangesphere.png)


Arnold also has some preset shaders. We made a glass cow: *right click on cow* **➜ Assign New Material ➜ Arnold ➜ Shader ➜ Surface ➜ AiStandardSurface ➜** *Rename to “cowShader”* **➜ Presets ➜ Glass ➜ Replace**.

![shader for cow](/lab04/images/cowshader1.png)
![shader for cow](/lab04/images/cowshader2.png)
![rendered cow](/lab04/images/glasscow.png)



## Optional: UV Mapping
We can also apply texture files to objects. First, create a new AiStandardSurface for the cylinder. Then, create a file node: **Attribute Editor ➜ cylinderShader  ➜ Base ➜ Color  ➜ checkerboard icon ➜ File**.

![create new surface and file node for cylinder](/lab04/images/newsurface.png)

Then, select your image: **Attribute Editor  ➜ File1 ➜ File Attribute  ➜ Image Name: Directory Icon  ➜** *select file*  **➜ Open**.

![select image](/lab04/images/selectimage.png)


If you render the scene now, you will notice that the texture has been applied but it is not mapped correctly. 

![texture applied wrong mapping](/lab04/images/wrongmapping.png)

To fix this, we will open the UV Editor: **UV ➜ UV Editor**.

![open uv editor](/lab04/images/uveditor.png)

Then select the cylinder in the outline. The texture file should appear. Inside the UV Editor, right click to bring up the quick change menu. Then still holding right click down, drag your cursor over “UV Shell”, and release. 

![uv shell](/lab04/images/uvshell.png)

There are three shells–top base, bottom base, and the side. Using the transform tools, align the shells to the image in the UV Editor.

![align uv shells](/lab04/images/alignshells.png)

It should now look like this. 

![better looking texture](/lab04/images/bettermapping.png)

The cylinder had nice a UV map to begin with. This is not always the case. For instance, the cow’s UV map looks like this. If you ever need to create your own UV maps, refer to [this video](https://www.youtube.com/watch?v=HLhazEa8wmw).

![cow uv map](/lab04/images/cowuv.png)

## Optional: Keyframe Animation
In this demo, we will make a 1 second clip of Wall-E moving. Note that there will be 24 frames per second. To make objects move, we need to set keys. First, make sure you are at frame 1 in the timeline. Then, move the object to its starting position. With the object still selected: **Channel Box/Layer Editor** ➜ Right click on **Translate X  ➜  Key Selected**. Repeat for Translate Z. There should now be a red bar at frame 1.

![making first keyframe](/lab04/images/firstkey.png)

 Next go to frame 24 in the timeline. Then, move the object to its end position and key the **Translate X** and **Translate Z** values again. Hit the play button to preview your animation. 
 
![first 24 frames](/lab04/images/previewanim.png)

To render the sequence, switch to **Rendering** in the dropdown menu. Then, open render setting: **Render ➜ Render Settings**. Change the **image format** to **tif**, **Frame/Animation ext** to **name.#.ext**, and **frame range** to **1 and 24**. Make note of the path as it is where your images will be saved.

![preparing to render the sequence](/lab04/images/preparesequence.png)

Then, instead of our usual Arnold Render View, we will use Render Sequence: **Render ➜ Render Sequence** (click on the square) **➜set current camera to stage: render cam➜ render sequence**. While rendering a sequence, you won’t be able to interact with Maya. To cancel a render sequence, hit escape.


Once done, you can import your images to an editing softwares and export as video or make gifs.

![rendered animation as gif](/lab04/images/renderedanim.gif)

You can also key things like scale and rotations. [Here’s a video](https://www.youtube.com/watch?v=RQ31vjgJM2c) that explains how to use the graph editor for more complex animations! 
# Conclusion
Feel free to further explore this software and share your work in our Slack’s “Gallery” Channel. There is an abundance of tutorials on Youtube. Other popular 3D computer graphics applications include Blender, Cinema4D and Houdini. If you enjoyed this lab, consider taking [CS125: Introduction to Computer Animation](http://cs.brown.edu/courses/cs125/) next fall!

