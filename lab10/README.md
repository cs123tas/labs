<div>

[]{.c17 .c45 .c21}

</div>

[Lab 8 -- Particles]{.c11 .c43}

[]{.c1}

[![](images/image13.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 411.79px; height: 286.50px;"}

# []{.c3} {#h.mvvjbcafmpyj .c33}

# [Prologue]{.c11 .c47} {#h.3h0beh2b3s2b .c41}

[In this lab you will implement your own particle system through one of
2 options. 1) by using your knowledge of framebuffers from the previous
lab or 2) by using your knowledge of Maya to create and shade a particle
system.]{.c1}

[]{.c1}

[Note: At this point, you've learned the basic foundations of OpenGL and
Maya! ]{.c11 .c24}[Of the last three labs, which will be helpful for
your final project, you will only need to complete two by November 30th
(but you can choose to do 3). ]{.c11 .c24}[You can get them checked off
at any point between now and November 30th. ]{.c11 .c24}

# [Intro]{.c3} {#h.y5pgntbatrit .c41}

[Particles are often used in computer graphics because they're easy to
implement, look great, and can give you lots of cool effects like fire
and smoke. Particles can be implemented in many ways. In this lab we
will explore how to implement them using either ]{.c11}[framebuffers
]{.c29}[and/or]{.c11}[ Maya's ]{.c29}[features. You will only need to
complete the FBO Version or the Maya version of this lab, but feel free
to do both!]{.c1}

[]{.c1}

[Particles -- FBO Version]{.c11 .c43}

[A common strategy to implement particle emission, is to keep a list of
particle positions and velocities on the CPU, update them ]{.c11}[one at
a time]{.c29}[, and then send their positions to the GPU to draw them.
But we know better. ]{.c11}

[]{.c1}

[Imagine a texture with RGBA float (positive and negative) texels. What
if we treated each texel as a particle? More precisely, each texel
represents one particle's XYZ position and its lifetime in seconds. In a
second texture, we can have the matching RGBA texel store XYZ velocity
and age in seconds. If we run a shader on an FBO with these two textures
as color attachments, we can update each particle in parallel!]{.c1}

[ ]{.c1}

[We will tackle this in a few steps:]{.c1}

-   [The first update initializes the particles' positions and
    velocities.]{.c1}
-   [Subsequent passes update the positions and velocities or reset them
    if the particle has died (its age is greater than its
    lifetime).]{.c1}
-   [The first draw pass takes the information in one FBO, updates each
    pixel, and places the new information in the other FBO.  ]{.c1}
-   [The second draw pass draws one triangle for each particle and, in
    the shader, moves the triangles to the position stored in the
    texture.]{.c1}

[ ]{.c1}

# [Getting Started]{.c3} {#h.kzd5u76bpobv .c41}

[The support code is the same as lab7 and this lab builds off of that
code. Be sure to finish lab7 before starting lab8. The tasks start at 13
for continuity with inline comments. Open up glwidget.cpp to get
started.]{.c1}

[ ]{.c1}

[Note that there is also a demo version of the lab that can be run by
inputting ]{.c11}[/course/cs1230/bin/cs1230_lab07_demo]{.c11 .c12}[ into
your terminal on the department machine through FastX3. Be sure to
toggle the "Particles" radio button.]{.c1}

[ ]{.c1}

[Particle Storm Clouds Brewing]{.c3}

[As with most labs, there is a bit of an overhead to getting something
to show up on screen so let's get that set up. Make sure to go through
each task carefully.]{.c1}

[ ]{.c1}

### [Task 13:]{.c29 .c35 .c52} {#h.iwnvbkle6ymq .c4 .c53}

[First, we are going to make the FBO member variables. Recall we are
using one for positions and one for velocities so we will need to make
two different FBOs. Let's start with initializeGL(), and initialize
m_particlesFBO1 and m_particlesFBO2 using
std::make_shared\<FBO\>(\...).]{.c1}

-   [They need 2 color attachments (position and velocity) but do not
    need depth attachments since it doesn't matter for the purposes of
    this lab.]{.c1}
-   [For width and height, pass in m_numParticles and 1.]{.c1}
-   [For wrap method, filter method, and storage type we will use
    TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
    TextureParameters::FILTER_METHOD::NEAREST, and GL_FLOAT.]{.c1}

[ ]{.c1}

[Task 14:]{.c5}

[Great! Now to update the particles. You can see we already set up some
of the logic for you in drawParticles(). prevFBO will hold the last
particle state. We will update the particles from there and draw them to
nextFBO.]{.c1}

-   [In drawParticles(), bind nextFBO.]{.c1}
-   [Note: we don't need to call glClear here because we know we're
    going to be overwriting all the pixels we care about.]{.c1}
-   [Use the program m_particleUpdateProgram]{.c1}
-   [Setup the textures to read from. We have two textures, so we need
    to tell which texture to put where:]{.c1}

```{=html}
<!-- -->
```
-   [Call glActiveTexture with GL_TEXTURE0.]{.c1}
-   [Bind prevFBO-\>getColorAttachment(0).]{.c1}
-   [Call glActiveTexture with GL_TEXTURE1.]{.c1}
-   [Bind prevFBO-\>getColorAttachment(1).]{.c1}

```{=html}
<!-- -->
```
-   [Send the uniforms to our shaders using
    ]{.c11}[glUniform\*\*\*]{.c11 .c12}[. We will use the following
    uniforms:]{.c1}

```{=html}
<!-- -->
```
-   [A ]{.c11}[float ]{.c11 .c24}[for whether or not it's the first
    pass, as GLSL doesn't support bools (pass in firstPass)]{.c1}
-   [An ]{.c11}[int ]{.c11 .c24}[for the number of particles
    (m_numParticles)]{.c1}
-   [Two textures, prevPos and prevVel. These are sent as
    ]{.c11}[ints]{.c11 .c24}[. Send 0 for prevPos and 1 for prevVel,
    representing the active texture bindings we used for them.]{.c1}

```{=html}
<!-- -->
```
-   [Draw a fullscreen quad using m_quad. Note: this will not draw to
    the size of the whole screen, but instead to the size of our FBO
    because of the glViewport call in FBO::bind(). This means that our
    shader will operate on each particle, as we wanted.]{.c1}

[ ]{.c1}

[Task 15:]{.c5}

[Let's fill in our update shader now. Most of it is provided. We just
need to fill in the update functions and the output locations. Let's
start with telling particles_update.frag which color attachments it
should output. At the top of particles_update.frag, declare the outputs,
pos and vel, with explicit locations. pos should have location 0 and vel
should have location 1, because those are the color attachments we are
using for them. Both are vec4s. To declare an explicit location for a
uniform, you do something like this:]{.c1}

-   [layout(location = ]{.c11}[\<desired location\>]{.c11 .c24}[) out
    ]{.c11}[\<output type\> \<output name\>]{.c11 .c24}[;]{.c1}

[ ]{.c17 .c29 .c38}

[Task 16:]{.c5}

[These texture attachments are not going to update themselves so let's
fill that out. Remember that the xyz component represent the physics
portion of an individual particle, whether it is position or velocity.
The w component of position and velocity will determine the particle's
lifetime and its age respectively.  In the same file:]{.c1}

-   [Fill in updatePosition by sampling prevPos and prevVel at uv and
    calculating pos + vel \* dt. The w component shouldn't change from
    prevPos, as it is the lifetime. This is the set amount of time the
    particle will show on the screen and is therefore constant.]{.c1}
-   [Fill in updateVelocity by sampling prevVel at uv and calculating
    vel + gravity \* dt. The w component should increase by dt, as it is
    the age. ]{.c1}

[ ]{.c17 .c29 .c38}

[Task 17:]{.c5}

[Great! Now we'll draw the particles to the screen. Back in
GLWidget::drawParticles:]{.c1}

-   [Unbind nextFBO so we're drawing to the default framebuffer]{.c1}
-   [Clear the color and depth buffers]{.c1}
-   [Use the program m_particleDrawProgram]{.c1}
-   [Call setParticleViewport(). This is a helper function that we
    provided that will center the viewport around {0,0} with an aspect
    ratio of 1:1 so the particles aren't distorted.]{.c1}
-   [Similar to before, set up our textures to read from:]{.c1}

```{=html}
<!-- -->
```
-   [glActiveTexture with GL_TEXTURE0]{.c1}
-   [Bind nextFBO-\>getColorAttachment(0)]{.c1}
-   [glActiveTexture with GL_TEXTURE1]{.c1}
-   [Bind nextFBO-\>getColorAttachment(1)]{.c1}

```{=html}
<!-- -->
```
-   [Set up the uniforms]{.c1}

```{=html}
<!-- -->
```
-   [An ]{.c11}[int]{.c11 .c24}[, 0, for pos, because it's
    GL_TEXTURE0]{.c1}
-   [An ]{.c11}[int]{.c11 .c24}[, 1, for vel, because it's
    GL_TEXTURE1]{.c1}
-   [An ]{.c11}[int]{.c11 .c24}[, m_numParticles, for numParticles]{.c1}

```{=html}
<!-- -->
```
-   [Draw one triangle for each particle.  To do this, we need a VAO,
    but it doesn't need any associated data or state, so we've prepared
    a simple one for you.]{.c1}

```{=html}
<!-- -->
```
-   [Call glBindVertexArray with m_particlesVAO]{.c1}
-   [Using glDrawArrays, draw 3 \* m_numParticles vertices using
    GL_TRIANGLES]{.c1}
-   [Unbind the VAO]{.c1}

```{=html}
<!-- -->
```
-   [Set the active texture back to GL_TEXTURE0]{.c1}

[ ]{.c1}

[It's Raining Particles]{.c3}

[Nice, almost there! We just need to fill in the particles_draw shaders!
But wait, we don't have any vertex data, we just know where the centers
of the particles should be. Luckily, we don't need to know the vertex
locations; we can just generate them in the vertex shader. Here are some
diagrams to explain what we're going to do:]{.c1}[![gpu
particles.png](images/image24.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 651.50px; height: 466.58px;"}

[]{.c1}

[Task 18:]{.c5}

[In particles_draw.vert:]{.c1}

-   [Fill in TRI_VERTS. These are our triangle offsets. See the diagram
    above.]{.c1}

```{=html}
<!-- -->
```
-   [We want to choose the offsets so that we can also use them as uv
    coordinates.]{.c1}
-   ["pos" is the "anchor" point \-- our particles position. We want the
    uv coordinate to be such that the square has texture coordinates
    going from 0 to 1 in both directions ({0,0} at pos). This can be
    seen by the blue square. Based on the diagram, figure out the XY
    values for p1, p2, and p3, and fill in TRI_VERTS accordingly.]{.c1}

```{=html}
<!-- -->
```
-   [Calculate particleID (the index of the particle we are on) and
    triID (the vertex of the current triangle that we are on). OpenGL
    provides an input called gl_VertexID. This tells us what vertex the
    shader is on, ranging from 0 to 3 \* numParticles.]{.c1}

```{=html}
<!-- -->
```
-   [particleID = gl_VertexID / 3 (floored)]{.c1}
-   [triID = gl_VertexID % 3 (remainder of gl_VertexID / 3)]{.c1}

```{=html}
<!-- -->
```
-   [Sample pos and vel to get posTime and velAge.  You can do this
    using texture if you calculate the uv coordinates yourself, or you
    can use texelFetch, like this:]{.c1}

```{=html}
<!-- -->
```
-   [texelFetch(]{.c11}[\<texture\>]{.c11 .c24}[, ivec2(particleID,
    0), 0)]{.c1}

[ ]{.c1}

[When you run your program now and switch to the "Particle" window, you
should see white triangles flying everywhere.]{.c11}

[![](images/image21.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 412.00px; height: 257.50px;"}

[If the particles aren't moving correctly or nothing is drawing, check
the output in Qt Creator for shader compilation errors or FBO errors. If
that didn't help, check that you used the correct
]{.c11}[glUniform\*\*\*]{.c11 .c12}[ calls, especially
]{.c11}[glUniform1f]{.c11 .c12}[ vs ]{.c11}[glUniform1i]{.c11 .c12}[. If
you send an integer to the GPU saying it is a float, the GPU will
interpret the bits as a float. This could cause some uniforms in your
shader to have junk values. Otherwise, you might need to go back and
check that you followed the steps correctly.]{.c1}

[  ]{.c1}

[Task 19:]{.c5}

[Once you have that working, there's just one last problem. Why are the
particles white triangles? We want nice colorful circles instead. In
particles_draw.frag, we don't want to color the fragments if our texture
coordinates are outside of a unit circle centered at {0.5, 0.5}. This is
why we did that complicated triangle offset earlier. We have provided
you with the method pickRainbowColor() that sends out a vec3 color you
can use.]{.c1}

-   [If the uv coordinate is within the unit circle (length(uv - 0.5) \<
    0.5), color it using the input "color" with an alpha of 1.
    Otherwise, call discard. "discard" tells the shader to not color
    this fragment.]{.c1}

[ ]{.c1}

[You're done! Congratulations! We skipped over a few of steps there, but
you should have a general sense of the idea behind GPU particles. We
encourage you to reuse the classes you filled in today for your final
project or any other project you want.]{.c1}

[ ]{.c1}

# [End]{.c3} {#h.5jjj2fc7g3ol .c41}

[Now you are ready to show your program to a TA to get checked
off!]{.c1}

[Be prepared to answer one or more of the following:]{.c1}

-   [Why is the width and height for the FBOs m_numParticles and 1
    respectively?]{.c1}
-   [Why is the nextFBO bound when the prevFBO's texture attachments are
    the ones being activated?]{.c1}
-   [We didn't use the depth attachment for this lab, but can you think
    of a situation where you may want to use it?]{.c1}

[ ]{.c1}

[Food for thought]{.c3}

[There's plenty of other cool tricks you can do if you want. None of
these are required for this lab:]{.c1}

-   [Color particles differently. We used a rainbow gradient, but you
    could do whatever! You could even properly texture map your
    particles using the uv coordinates (that's why we made them go from
    0 to 1 in that square around the particle).]{.c1}
-   [Change particle radius dynamically. We made the particle grow when
    they are born and shrink when they die, but there's plenty more you
    could do!]{.c1}
-   [Have more interesting update methods. You can add additional forces
    to make more complicated behavior, or update the particles to move
    along a specific curve.]{.c1}
-   [Store particles in a 2D array instead of a 1D array. This would
    allow you to add more particles, as a 1 dimensional texture isn't
    very optimal. OpenGL has a limit on render buffer dimension that
    will vary from machine to machine. It is usually somewhere between
    8000 and 17000. This means that, using our current method of 1D
    indexing, you can only support that many particles. With 2D
    indexing, the number of particles you could store would be squared,
    allowing you to store millions of particles instead of our preset
    5000!]{.c1}
-   [Draw particles in world space instead of screen space. In this lab,
    we drew our particles in the space from -1 to 1 in each dimension.
    In fact, our particles all had a z position of 0, so they were
    basically 2D particles. To draw 3D particles, you would instead need
    to transform the particle triangles from world space to screen
    space. This is slightly more complicated, because you would also
    need to handle billboarding, or making the particles all face the
    camera to give the illusion of spherical particles.]{.c1}

[ ]{.c1}

[Check out some of these examples:]{.c1}

-   [[https://threejs.org/examples/?q=particle\#webgl_gpu_particle_system](https://www.google.com/url?q=https://threejs.org/examples/?q%3Dparticle%23webgl_gpu_particle_system&sa=D&source=editors&ust=1630507972544000&usg=AOvVaw3DAjD1MyqdJAQl63y7wkwN){.c14}]{.c18
    .c11}
-   [[https://lab.hakim.se/trail/02/](https://www.google.com/url?q=https://lab.hakim.se/trail/02/&sa=D&source=editors&ust=1630507972545000&usg=AOvVaw1m7LRCnC77WvwWc8F2gKa-){.c14}]{.c11
    .c18}
-   [[https://www.youtube.com/watch?v=CMVRILPTWzU](https://www.google.com/url?q=https://www.youtube.com/watch?v%3DCMVRILPTWzU&sa=D&source=editors&ust=1630507972545000&usg=AOvVaw2okLwp0HuENJ8ihs-tMF11){.c14}]{.c18
    .c11}

[[](https://www.google.com/url?q=https://www.youtube.com/watch?v%3DCMVRILPTWzU&sa=D&source=editors&ust=1630507972545000&usg=AOvVaw2okLwp0HuENJ8ihs-tMF11){.c14}]{.c18
.c11 .c35 .c46}

[  ]{.c1}

[Particles -- Maya Version]{.c11 .c43}^[\[a\]](#cmnt1){#cmnt_ref1}^

[In lab (??) we explored how to use Maya. Maya is useful for creating
complex visual effects because of its (??). It has lots of features for
simulating particles and other phenomena. It also has a python (and MEL-
]{.c11}[Maya Embedded Language) ]{.c11}[scripting API which allows you
to create complex scenes and effects with a few lines of code. In this
section of the lab, we will use Maya's GUI to implement a colorful
particle emitter, and then replicate the scene using python scripting.
In the last part of the lab we will use our programming knowledge to
create cool particle-based effects within Maya using a python
script.]{.c11}

[Part 1: Creating a shaded particle emitter in maya]{.c16 .c17}

[]{.c1}

[Task 1: Create a particle emitter]{.c29}

[]{.c1}

1.  [In the FX tab in Maya, create an emitter. Maya has created
    "emitter" objects for you that you can create to model things like a
    stream of particles to simulate fireworks or explosions. To create
    one in the application you have to select the FX tab and then the
    circled "emitter" button.]{.c1}

[![](images/image8.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 61.33px;"}

2.  [When you create the emitter, a properties tab should appear which
    will allow you to configure your particle emitter.]{.c1}

[![](images/image23.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 276.00px;"}

1.  [Position your emitter at X=0; Y=5; Z=0]{.c1}

[![](images/image10.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 269.08px; height: 261.50px;"}

[]{.c1}

3.  [Toggle the play button on, you should see a downward stream of
    particles]{.c1}

[![](images/image15.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 330.67px;"}

[]{.c1}

[Task 2: Manipulate Particle Direction]{.c29}

[]{.c1}

[We will manipulate particle direction by disabling gravity and using an
external force field. To do this:]{.c1}

[]{.c1}

1.  [Hit pause on the particle emitter mid-emission and select the
    particle stream. On the left half of your screen, navigate to the
    nParticleShape tab, and check ignore solver gravity. This should
    make your particles emit in every direction.]{.c1}

[![](images/image26.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 248.00px;"}

[]{.c1}

[After disabling gravity:]{.c1}

[]{.c1}

[![](images/image17.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 264.76px; height: 171.50px;"}

[]{.c1}

[]{.c1}

2.  [To create a force field, toggle your menu to FX]{.c1}

[![](images/image1.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 89.33px;"}

[Navigate to Fields/Solvers and click on Turbulence]{.c1}

[![](images/image11.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 105.33px;"}

[Confirm the default magnitude value of 15, and hit apply]{.c1}

[![](images/image22.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 334.63px; height: 310.50px;"}

[Now when you hit play, the motion of your particles should change from
a uniform 360 degree emission, to emission in the direction of your
newly added force field]{.c1}

[![](images/image27.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 348.50px; height: 262.55px;"}

[]{.c1}

[Task 3: Shading]{.c29}

[]{.c1}

1.  [First add a light to the scene: ]{.c1}

```{=html}
<!-- -->
```
1.  [Navigate back to the modeling menu and toggle the rendering
    tab]{.c1}

[]{.c1}

[![](images/image12.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 98.67px;"}

[]{.c1}

2.  [Add a directional light to the scene and configure its properties
    to face the particle emitter]{.c1}

[![](images/image2.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 262.67px;"}

[]{.c1}

2.  [Configure the light for the arnold shader by navigating to the
    directionalLightShape tab, going to the arnold menu, and setting the
    exposure to 1 and angle to 10]{.c1}

[![](images/image28.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 486.94px; height: 409.69px;"}

[]{.c1}

1.  [Add an arnold surface shader by right clicking on the particle
    emission and selecting 'Assign New Material']{.c1}

[![](images/image19.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 623.50px; height: 390.69px;"}

[]{.c1}

2.  [In the material menu select aiStandardSurface in the Arnold Surface
    section]{.c1}

[![](images/image5.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 286.94px; height: 726.50px;"}

[]{.c1}

3.  [Next let's navigate to the hypershade node editor]{.c1}

[![](images/image16.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 108.00px;"}

4.  [In the node editor, select the aiStandardShader we just applied to
    our particles]{.c1}

[![](images/image25.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 333.33px;"}

[]{.c1}

5.  [To bring the aiStandardSurface to the graph editor, right click on
    it in the materials menu and drag your mouse to the graph editor.
    When you release your mouse, the node should show up in the node
    editor.]{.c1}

[]{.c1}

[![](images/image4.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 390.67px;"}

[]{.c1}

6.  [In the node creator menu, search for jitter, aiColorJitter should
    show up. Double click on it. This should add the color jitter node
    to the graph editor]{.c1}

[![](images/image6.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 298.67px;"}

[]{.c1}

7.  [Connect the color jitter's out color to the aiStandardSurface's
    base color]{.c1}

[![](images/image18.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 376.00px;"}

8.  [Navigate to the Material viewer on the right half of the screen.
    Set it up to preview arnold shader colors by selecting the arnold
    option on the first dropdown]{.c1}

[![](images/image20.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 734.67px;"}

[]{.c1}

9.  [Toggle type to face]{.c1}

[![](images/image3.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 888.00px;"}

10. [To randomize colors, change the input from grey to a different base
    color, set the hue min to -1 and hue max to +1, saturation min and
    max to 10.]{.c1}

[![](images/image14.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 850.67px;"}

11. [Exit the hypershade menu]{.c1}
12. [Switch your renderer to Arnold]{.c1}

[![](images/image29.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 490.50px; height: 672.87px;"}

13. [Hit play on the arnold viewportrenderer. This should shade your
    scene with the hypershade parameters we set]{.c1}

[![](images/image9.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 333.50px; height: 261.35px;"}

[]{.c1}

[One frame of the shaded scene:]{.c5}

[![](images/image7.png)]{style="overflow: hidden; display: inline-block; margin: 0.00px 0.00px; border: 0.00px solid #000000; transform: rotate(0.00rad) translateZ(0px); -webkit-transform: rotate(0.00rad) translateZ(0px); width: 624.00px; height: 528.00px;"}

[]{.c1}

[Part 2: Creating a particle emitter with a python script.]{.c16}

[]{.c1}

[Now that we have manually created a colorful particle emitter, we will
replicate the same scene using python. A link to the maya python API is
here:
]{.c11}[[https://download.autodesk.com/us/maya/2009help/api/main.html](https://www.google.com/url?q=https://download.autodesk.com/us/maya/2009help/api/main.html&sa=D&source=editors&ust=1630507972554000&usg=AOvVaw0x8RCdlIuiojuM0kNyR9sp){.c14}]{.c18
.c11}

[]{.c1}

1.  [Create a new scene]{.c1}
2.  [Open the maya script editor]{.c1}
3.  [Flip to the python tab]{.c1}
4.  [Import the necessary maya scripting packages]{.c1}

[import]{.c11 .c34}[ ]{.c10}[maya]{.c10 .c12}[.]{.c10}[cmds]{.c10
.c12}[ ]{.c10}[as]{.c11 .c34}[ ]{.c10}[cmds]{.c10 .c12 .c30}

[from]{.c11 .c34}[ ]{.c10}[mtoa]{.c10 .c12}[.]{.c10}[cmds]{.c10
.c12}[.]{.c10}[arnoldRender]{.c10 .c12}[ ]{.c10}[import]{.c11
.c34}[ ]{.c10}[arnoldRender]{.c10 .c30 .c12}

5.  [Create a particle emitter]{.c1}

[]{.c1}

[cmds]{.c10 .c12}[.]{.c10}[emitter]{.c10 .c12}[(]{.c10}[pos]{.c10
.c12}[=]{.c11 .c15}[(]{.c10}[0]{.c22 .c11}[,]{.c10}[5]{.c22
.c11}[,]{.c10}[0]{.c22 .c11}[), ]{.c10}[type=]{.c11 .c15}[\"omni\"]{.c23
.c11}[, ]{.c10}[rate]{.c10 .c12}[=]{.c11 .c15}[500]{.c22 .c11}[,
]{.c10}[n]{.c10 .c12}[=]{.c11 .c15}[\"emitter\"]{.c23 .c11}[)]{.c10
.c27}

[cmds]{.c10 .c12}[.]{.c10}[particle]{.c10 .c12}[( ]{.c10}[n]{.c10
.c12}[=]{.c11 .c15}[\'emitted\']{.c23 .c11}[ )]{.c10 .c27}

[particle_emitter]{.c10 .c12}[ ]{.c10}[=]{.c11 .c15}[ ]{.c10}[cmds]{.c10
.c12}[.]{.c10}[connectDynamic]{.c10 .c12}[( ]{.c10}[\'emitted\']{.c23
.c11}[, ]{.c10}[em]{.c10 .c12}[=]{.c11 .c15}[\'emitter\']{.c23
.c11}[ )]{.c10 .c27}

[\# Creates a particle emitter.]{.c25 .c11 .c12}

6.  [Add turbulence forces to influence the motion of the particle]{.c1}

[cmds]{.c10 .c12}[.]{.c10}[turbulence]{.c10 .c12}[( ]{.c10}[n]{.c10
.c12}[=]{.c11 .c15}[\'turbulence\']{.c11 .c23}[, ]{.c10}[m]{.c10
.c12}[=]{.c11 .c15}[15.0]{.c22 .c11}[)]{.c10 .c27}

[cmds]{.c10 .c12}[.]{.c10}[connectDynamic]{.c10
.c12}[(]{.c10}[particle_emitter]{.c10 .c12}[, ]{.c10}[f]{.c10
.c12}[=]{.c11 .c15}[\'turbulence\']{.c23 .c11}[)]{.c10 .c27}

[\# Adds an external turbulence force to influence the motion of the
particles.]{.c25 .c11 .c12}

[​]{.c10 .c27}

7.  [Add a directional light to the scene]{.c1}

[light]{.c10 .c12}[ ]{.c10}[=]{.c11 .c15}[ ]{.c10}[cmds]{.c10
.c12}[.]{.c10}[directionalLight]{.c10 .c12}[(]{.c10}[rotation]{.c10
.c12}[=]{.c11 .c15}[(]{.c10}[-]{.c11 .c15}[45]{.c22 .c11}[,
]{.c10}[-]{.c11 .c15}[60]{.c22 .c11}[, ]{.c10}[-]{.c11 .c15}[80]{.c11
.c22}[), ]{.c10}[position]{.c10 .c12}[=]{.c11 .c15}[(]{.c10}[17]{.c22
.c11}[, ]{.c10}[16]{.c22 .c11}[, ]{.c10}[15]{.c22 .c11}[),
]{.c10}[intensity]{.c10 .c12}[=]{.c11 .c15}[1.0]{.c22 .c11}[,
]{.c10}[n]{.c10 .c12}[=]{.c11 .c15}[\'light\']{.c23 .c11}[)]{.c10 .c27}

[\# Adds a light to the scene]{.c25 .c11 .c12}

8.  [Configure the light for the arnold shader]{.c1}

[​]{.c10 .c27}

[cmds]{.c10 .c12}[.]{.c10}[setAttr]{.c10
.c12}[(]{.c10}[\'light.aiExposure\']{.c23 .c11}[, ]{.c10}[1]{.c22
.c11}[)]{.c10 .c27}

[cmds]{.c10 .c12}[.]{.c10}[setAttr]{.c10
.c12}[(]{.c10}[\'light.aiAngle\']{.c23 .c11}[, ]{.c10}[10]{.c22
.c11}[)]{.c10 .c27}

[\# Configures the light for Arnold shader]{.c25 .c11 .c12}

9.  [Add an arnold surface shader for the particles]{.c1}

[]{.c1}

[​]{.c10 .c27}

[particleShader]{.c10 .c12}[ ]{.c10}[=]{.c11 .c15}[ ]{.c10}[cmds]{.c10
.c12}[.]{.c10}[shadingNode]{.c10
.c12}[(]{.c10}[\'aiStandardSurface\']{.c23 .c11}[,
]{.c10}[asShader]{.c10 .c12}[=]{.c11 .c15}[True]{.c11 .c34}[)]{.c10
.c27}

[cmds]{.c10 .c12}[.]{.c10}[select]{.c10 .c12}[(]{.c10}[\'emitted\']{.c23
.c11}[)]{.c10 .c27}

[cmds]{.c10 .c12}[.]{.c10}[hyperShade]{.c10 .c12}[(]{.c10}[assign]{.c10
.c12}[=]{.c11 .c15}[particleShader]{.c10 .c12}[)]{.c10 .c27}

[\# Adds Arnold Surface shader for particles]{.c25 .c11 .c12}

10. [Add color jittering to randomize particle color]{.c1}

[colorJitter]{.c10 .c12}[ ]{.c10}[=]{.c11 .c15}[ ]{.c10}[cmds]{.c10
.c12}[.]{.c10}[shadingNode]{.c10 .c12}[(]{.c10}[\'aiColorJitter\']{.c23
.c11}[, ]{.c10}[asShader]{.c10 .c12}[=]{.c11 .c15}[True]{.c11
.c34}[)]{.c10 .c27}

[cmds]{.c10 .c12}[.]{.c10}[connectAttr]{.c10 .c12}[(
]{.c10}[colorJitter]{.c10 .c12}[+]{.c11 .c15}[\'.outColor\']{.c23
.c11}[, ]{.c10}[particleShader]{.c10 .c12}[+]{.c11
.c15}[\'.baseColor\']{.c23 .c11}[, ]{.c10}[force]{.c10 .c12}[=]{.c11
.c15}[True]{.c11 .c34}[)]{.c10 .c27}

[\# Adds Color jittering (to randomize the particle colors)]{.c11 .c12
.c25}

11. [Set the default color of the particles to aqua]{.c1}

[cmds]{.c10 .c12}[.]{.c10}[setAttr]{.c10
.c12}[(]{.c10}[colorJitter]{.c10 .c12}[+]{.c11 .c15}[\".input\"]{.c23
.c11}[, ]{.c10}[0]{.c22 .c11}[, ]{.c10}[1]{.c22 .c11}[, ]{.c10}[1]{.c22
.c11}[, ]{.c10}[type=]{.c11 .c15}[\"double3\"]{.c23 .c11}[)]{.c10 .c27}

[\# Sets default color of particle to aqua]{.c25 .c11 .c12}

[​]{.c10 .c27}

12. [Set the color range to the entire spectrum]{.c1}

[cmds]{.c10 .c12}[.]{.c10}[setAttr]{.c10
.c12}[(]{.c10}[colorJitter]{.c10 .c12}[+]{.c11
.c15}[\".faceHueMin\"]{.c23 .c11}[, ]{.c10}[1]{.c22 .c11}[)]{.c10 .c27}

[cmds]{.c10 .c12}[.]{.c10}[setAttr]{.c10
.c12}[(]{.c10}[colorJitter]{.c10 .c12}[+]{.c11
.c15}[\".faceHueMax\"]{.c23 .c11}[, ]{.c10}[-]{.c11 .c15}[1]{.c22
.c11}[)]{.c10 .c27}

[\# Sets color range to entire spectrum]{.c25 .c11 .c12}

[​]{.c10 .c27}

13. [Set the saturation to max]{.c1}

[cmds]{.c10 .c12}[.]{.c10}[setAttr]{.c10
.c12}[(]{.c10}[colorJitter]{.c10 .c12}[+]{.c11
.c15}[\".faceSaturationMin\"]{.c23 .c11}[, ]{.c10}[10]{.c22
.c11}[)]{.c10 .c27}

[cmds]{.c10 .c12}[.]{.c10}[setAttr]{.c10
.c12}[(]{.c10}[colorJitter]{.c10 .c12}[+]{.c11
.c15}[\".faceSaturationMin\"]{.c23 .c11}[, ]{.c10}[10]{.c22
.c11}[)]{.c10 .c27}

[\# Sets default color saturation to highest]{.c25 .c11 .c12}

[​]{.c10 .c27}

[]{.c1}

14. [Set the camera position to a good place to capture the scene]{.c1}

[cmds]{.c10 .c12}[.]{.c10}[camera]{.c10 .c12}[(]{.c10}[\'persp\']{.c23
.c11}[, ]{.c10}[e]{.c10 .c12}[=]{.c11 .c15}[True]{.c11 .c34}[,
]{.c10}[position]{.c10 .c12}[=]{.c11 .c15}[(]{.c10}[1.5]{.c22 .c11}[,
]{.c10}[6]{.c22 .c11}[, ]{.c10}[4.5]{.c22 .c11}[),
]{.c10}[rotation]{.c10 .c12}[=]{.c11 .c15}[(]{.c10}[-]{.c11
.c15}[17]{.c22 .c11}[,]{.c10}[375]{.c22 .c11}[,]{.c10}[0]{.c22
.c11}[))]{.c10 .c27}

[\# Set the camera position to a good place to capture the scene]{.c25
.c11 .c12}

15. [Hit play on the scene and scub to a timestamp]{.c1}
16. [Enable the arnold renderer and hit play!]{.c1}

[]{.c1}

[Part 3: Taking Advantage of Maya Scripting]{.c16}

[]{.c1}

[So far we have created and ]{.c11}[shaded]{.c11}[ a single particle
emitter. To explore the benefits of maya scripting, please demonstrate a
scene with multiple particle emitters, possibly interacting with each
other. Our example includes multiple particle emitters moving around and
emitting particles when they come close to each other. You can fill in
the todos below to emulate the same scene or build your own! ]{.c1}

[]{.c1}

[import maya.cmds as cmds]{.c19 .c17}

[import random]{.c19 .c17}

[import math]{.c19 .c17}

[]{.c19 .c17}

[]{.c19 .c31}

[def moveEmitters(emitters):]{.c19 .c17}

[    for instanceResult in emitters:]{.c19 .c17}

[        x = random.uniform(-10, 10)]{.c19 .c17}

[        y = random.uniform(0, 20)]{.c19 .c17}

[        z = random.uniform(-10, 10)]{.c19 .c17}

[]{.c19 .c17}

[        cmds.move(x, y, z, instanceResult)]{.c19 .c17}

[]{.c19 .c31}

[]{.c19 .c31}

[def createAndGetEmitters():]{.c19 .c17}

[    emittersToReturn = \[\]]{.c19 .c17}

[/]{.c42}[/ TODO 1: Create 50 particle emitters using a for loop. Each
particle emitter should have a unique name.]{.c19 .c20}

[//HINT 1: see the previous section on how to create and name a particle
emitter via scripting (step 5)]{.c19 .c20}

[//HINT 2: to set the rate use this command e.g:
​​cmds.setAttr(\"%s.rate\" % emitter1\[1\], 300)]{.c19 .c20}

[]{.c19 .c20}

[]{.c19 .c17}

[        emittersToReturn.append(particleEmitter)]{.c19 .c17}

[    return emittersToReturn]{.c19 .c17}

[]{.c19 .c31}

[]{.c19 .c31}

[def getDistance(emitter1, emitter2):]{.c19 .c17}

[    diffX = cmds.getAttr(\"%s.translateX\" % emitter1\[0\]) -
cmds.getAttr(\"%s.translateX\" % emitter2\[0\])]{.c19 .c17}

[    diffY = cmds.getAttr(\"%s.translateY\" % emitter1\[0\]) -
cmds.getAttr(\"%s.translateY\" % emitter2\[0\])]{.c19 .c17}

[    diffZ = cmds.getAttr(\"%s.translateZ\" % emitter1\[0\]) -
cmds.getAttr(\"%s.translateZ\" % emitter2\[0\])]{.c19 .c17}

[]{.c17 .c19}

[    return math.sqrt((diffX \* diffX) + (diffY \* diffY) + (diffZ \*
diffZ))]{.c19 .c17}

[]{.c19 .c17}

[]{.c19 .c17}

[def emitOnCloseContact(emitters):]{.c19 .c17}

[    for emitter1 in emitters:]{.c19 .c17}

[        for emitter2 in emitters:]{.c19 .c17}

[//TODO 2: if the distance between 2 emitters is less than 1, tell the
emitters to emit particles, if not, tell them to not emit
particles]{.c19 .c20}

[//Hint 1: use the getDistance method provided to compute distance, make
sure you aren't comparing the distance between the same emitter]{.c19
.c20}

[//Hint 2: ]{.c19 .c20}

[]{.c19 .c17}

[emitters = createAndGetEmitters()]{.c19 .c17}

[]{.c19 .c17}

[for frame in range(1, 80):]{.c19 .c17}

[    cmds.currentTime(frame)]{.c19 .c17}

[    moveEmitters(emitters)]{.c19 .c17}

[    emitOnCloseContact(emitters)]{.c19 .c17}

[]{.c1}

[]{.c1}

::: {.c54}
[\[a\]](#cmnt_ref1){#cmnt1}[This needs some intro motivational
text\--why Maya is useful for creating complex visual effects, how it
has lots of features for simulating particles and other phenomena, how
we can use our programming knowledge to create cool particle-based
effects within Maya.]{.c17 .c21 .c45}
:::
