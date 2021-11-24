# Lab 11 - Dielectric Materials

In this lab, we’ll cover how we simulate a special type of material called a dielectric in real-time rendering.

We’ll also cover how to simulate metallic surfaces more realistically. By the end of the lab, you should be able to create interesting new surface types that may come in handy for the final project.

Relevant reading: Sections 26.4 + 5 and 27.8 in Computer Graphics: Principles and Practice

## What is a dielectric? 
Dielectrics are materials that are poor conductors of electricity. Their degree of insulation is measured by a number called the dielectric constant (denoted <img src="https://render.githubusercontent.com/render/math?math=\varepsilon_r">), defined as the ratio between the permittivity of a material and the permittivity of a perfect vacuum. Lower constants imply more insulating materials. 

Here are the dielectric constants of a few common materials:
| Material      | Dielectric constant (room temp.) |
| ----------- | ----------- |
|Water | 80.2|
| Pyrex glass | 4.7|
| Graphite | 10-15|
| Rubber | 7|
|Teflon | 2.1|
| Metal | infinite! | 

But why do we care about how insulating a particular material is in computer graphics?

When light hits an interface between two media (e.g. air and glass), it **refracts** based on the **index of refraction** of the two media. And how do we figure out the index of refraction (denoted <img src="https://render.githubusercontent.com/render/math?math=\eta">)?

### Refraction at air-glass and glass-air interfaces
<img src="https://upload.wikimedia.org/wikipedia/commons/8/85/Refraction_photo.png">

It turns out that for most materials that we care about in graphics, the index of refraction is defined as:

<img src="https://render.githubusercontent.com/render/math?math=\eta = \sqrt{\varepsilon_r}" justify="center" width="33%">

## Snell’s Law
We can relate the indices of refraction of two materials to how light will refract at their interface using Snell’s Law.

<img src="https://render.githubusercontent.com/render/math?math=\frac{\sin{\theta_1}}{\sin{\theta_2}} = \frac{\eta_2}{\eta_1}" justify="center" width="30%">


We can calculate the refraction of light at an interface if we know the indices of refraction of the two incident materials and the angle of the ray of light entering the interface.

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/3f/Snells_law2.svg/800px-Snells_law2.svg.png" width="66%" justify="center">

Now we can bend light! The amount it bends relates back to the dielectric constant of the medium it exits and the medium it enters. 

We’re not quite done determining how to simulate glassy surfaces. If you’ve ever looked at a snowglobe or another glass object, you’ve probably also seen some light reflected off the surface. We’ll need a bit more physics to think about this phenomenon.

## Fresnel Reflectance
Assuming the material we’re simulating does not absorb light, all light that hits the surface of the material must either be transmitted or reflected due to conservation of energy. But how do we know how much light gets transmitted and how much gets reflected? 

It depends on the _polarization_ of the light, which refers to whether the light wave lies in the plane of the incident surface (P-polarization) or is perpendicular to the incident surface (S-polarization). Fresnel’s Law describes the reflectance, or percentage of incident light reflected, for each type of polarized light.

We include the original Fresnel equations below - they’ll come in handy soon!

<img src="https://render.githubusercontent.com/render/math?math=R_s = \left|\frac{\eta_1\cos\theta_i - \eta_2\cos\theta_t}{\eta_1\cos\theta_i %2B \eta_2\cos\theta_t}\right|^2" justify="center" width="66%">

<img src="https://render.githubusercontent.com/render/math?math=R_p = \left|\frac{\eta_2\cos\theta_i - \eta_1\cos\theta_t}{\eta_2\cos\theta_i %2B \eta_1\cos\theta_t}\right|^2" justify="center" width="66%">

In graphics, it’s common to assume that light is unpolarized, which means that we can calculate a single reflectance value for a given ray of light. Assuming light is unpolarized means that the overall percentage of light reflected rather than transmitted is 

<img src="https://render.githubusercontent.com/render/math?math=R_F = (R_s %2b R_p) / 2" justify="center" width="66%">

Notice that this value depends on the angle of incidence of the ray. As with the simulation of many physical phenomena in computer graphics, we’re going to leverage a good approximation of Fresnel reflectance to reduce the number of calculations we need to perform. 

## Schlick’s Approximation
Schlick (1994) introduced an approximation of Fresnel reflectance that is widely used in real-time rendering.


<img src="https://render.githubusercontent.com/render/math?math=R_F = R_0 %2b (1-R_0)(1-\cos\theta)^5" justify="center" width="66%">

where 

<img src="https://render.githubusercontent.com/render/math?math=R_0 = \left(\frac{\eta_1 - \eta_2}{\eta_1 %2b \eta_2}\right)^2" justify="center" width="66%">

<img src="https://render.githubusercontent.com/render/math?math=\cos \theta = n \cdot (-\omega)" justify="center" width="66%">

for <img src="https://render.githubusercontent.com/render/math?math=n"> the surface normal and <img src="https://render.githubusercontent.com/render/math?math=\omega">  the vector from the light source to the surface.

Because <img src="https://render.githubusercontent.com/render/math?math=R_0"> doesn’t depend on the angle at which the light hits the surface, we can calculate it once per surface and store it.


We can now calculate both the percentage of light reflected off of a surface and the direction that light will travel when transmitted through the surface. This is everything we need to know to simulate a glassy surface! Almost :) 

One last important note: we weren’t entirely honest earlier when we said there’s a single index of refraction for a material. The index of refraction varies based on the wavelength of the light incident to the surface.

## Simulating metal
We’ve already seen how to simulate smooth, plasticky surfaces using the Phong model, but what if we want to simulate shiny surfaces that aren’t so smooth? 

Imagine a copper table top or aluminum foil. You’ll notice these rougher materials have specular highlights, but they’re more spread out most of the time. We’re going to implement a lighting model for these surfaces that takes into account their physical properties to make our simulation more realistic. 

## Cook-Torrance Model
The Phong model assumes surfaces are uniformly smooth (and all equally so). This assumption is weak. If you touch a rusty piece of metal, it’s definitely going to be rougher than a soda can! 

The roughness of a surface affects how it forms specular reflections because of **microfacets**, or small, jagged valleys in the surface that bounce light around on a microscopic scale. 

These microfacets can affect the intensity of the specular reflection in a given direction. This means that the distribution of specular reflected light will be less uniform than the mirror-based approach of the Phong model. 

Cook-Torrance assumes that ambient illumination is calculated as before and diffuse illumination is Lambertian (calculated using the same equation as in Intersect and Ray)

All that remains is to calculate the specular illumination. 

We first give the full Cook-Torrance formulation for specular illumination: 

<img src="https://render.githubusercontent.com/render/math?math=R_s(\omega, \omega_r) = \frac{F(\omega)}{\pi}\cdot\frac{DG}{\pi(n\cdot(-\omega))(n\cdot\omega_r)}" justify="center" width="66%">

- <img src="https://render.githubusercontent.com/render/math?math=F"> represents the Fresnel reflectance function we defined earlier.
- <img src="https://render.githubusercontent.com/render/math?math=D"> represents the slope distribution function, which describes the fraction of microfacets that are oriented in each direction
- <img src="https://render.githubusercontent.com/render/math?math=G">  represents the geometric attenuation factor, which accounts for masking and shadowing of facets
- <img src="https://render.githubusercontent.com/render/math?math=\omega">  is the direction vector from the eye to the surface
- <img src="https://render.githubusercontent.com/render/math?math=\omega_r"> is the mirror reflection direction vector

There are many possible choices of slope distribution function. Cook and Torrance used the Beckmann variety:

<img src="https://render.githubusercontent.com/render/math?math=D(\alpha) = \frac{1}{m^2\cos^4(\alpha)}e^{-\left[\frac{\tan\alpha}{m}\right]^2}" justify="center" width="300px">

where <img src="https://render.githubusercontent.com/render/math?math=\alpha = \cos^{-1}(n\cdot(-\omega))"> 

Adjusting <img src="https://render.githubusercontent.com/render/math?math=m"> approximates the roughness of the surface. Higher <img src="https://render.githubusercontent.com/render/math?math=m"> corresponds to a rougher surface. You may also read about the GGX distribution function, which was developed after the introduction of the Beckmann function and provides slightly more realistic specular reflections for certain surfaces.

Geometric attenuation is written in terms of the half-angle vector <img src="https://render.githubusercontent.com/render/math?math=h = (-\omega%2b\omega_r)/2">


<img src="https://render.githubusercontent.com/render/math?math=G(\omega, \omega_r) = \mathrm{min}\{1, 2\frac{(n\cdot h)(n\cdot(-\omega))}{(-\omega)\cdot h}, 2\frac{(n\cdot h)(n\cdot\omega_r)}{\omega_r\cdot h}\}" justify="center" width="600px">

# Real-time Implementation
There’s a lot of support code, but you only need to worry about `metal.frag` and `glass.frag`.

<img src="https://i.imgur.com/5COMFJC.png">
Metal and glass shaders applied to a dragon model

## Cube Environment Mapping
Notice that both of these materials interact with the scene by reflecting and/or refracting it. For now, simulating this perfectly is too hard, so we'll approximate using a technique called environment mapping.

Environment mapping encodes all data that can be reflected or refracted into a texture. For example, to simulate reflection, we simply figure out where in the scene the reflected data should come from and sample the texture (the environment map) correspondingly.

Though computationally cheap, this technique only allows objects to reflect the environment: objects do not show reflections of each other. **This technique can be extended to raytracing by simply recursively tracing the refracted ray to obtain its lighting contribution!**

In this lab you will encode the environment in a cube map. Cube maps are a collection of six texture maps that form a cube, where each inside face of the cube is covered with a 2D texture:

<img src="https://i.imgur.com/mJ1lufj.png">
Example of an environment cube map, unrolled into a single image

Each texture map is chosen so that the sides of the cube come together to form an entire scene. The object we're shading is surrounded by a skybox that uses the cube map textures. This way, the object being rendered appears to be inside the environment and the material shader can index into the environment cube map to determine what data to reflect and/or refract.

Since the skybox and the shader will use the same cube map, it will appear as though the object is reflecting and refracting the scene. Remember to use the same cube map for the skybox and the shader! Otherwise, your object will be reflecting a different scene than the one it's in.

The skybox has already been written for you, but the shaders that index into the cube map will be up to you.

## Metal Shader

To implement the metal shader in `metal.frag`, follow these high-level steps:
1. Determine the diffuse and ambient terms as you would in a Phong shader.
2. For the specular term, <img src="https://render.githubusercontent.com/render/math?math=k_s"> use the Cook-Torrance model.
    - Clamp it above 0, so that negative values are ignored
3. Compute the object color (without the reflection component) as `ambient` + `lambertTerm` * `diffuse` + <img src="https://render.githubusercontent.com/render/math?math=k_s"> * `specular`.
4. Determine the reflection color.
    - Calculate the reflected direction using `cameraToVertex`, the surface normal, and the GLSL `reflect` function.
    - Convert the reflected direction to world space
    - Sample the cube map. Qt might not recognize the texture() function—don’t worry, just compile and run the code, it should work fine.
    - Don’t forget how we transform normals!
5. Calculate the reflectance <img src="https://render.githubusercontent.com/render/math?math=F"> by Schlick's approximation. Remember to negate `cameraToVertex` to ensure that it points away from the vertex, not toward it.
6. Get the final color by blending the reflection color and the object color
    - Use the formula: `objectColor` (<img src="https://render.githubusercontent.com/render/math?math=1-F">) + `reflectionColor`(<img src="https://render.githubusercontent.com/render/math?math=F">)
    - You might consider using GLSL's `mix` function for this

Note: the default ambient color is `(0.7, 0.7, 0.7)`. This will make the metal sphere look really blown out! You can change the color in the panel on the right hand side of the GUI. This will make the sphere look more like what’s in the following picture.

<img src="https://imgur.com/s7G2dUi.png">

## Glass Shader

Our strategy for refraction will be similar to reflection: using the GLSL `refract` function, compute the direction of the incident vector that refracts in the direction of the eye vector, and then sample the cube map to simulate the incoming light.

The following refraction shader samples a refracted ray from the surrounding cube environment map:

```glsl
// Fragment shader
uniform samplerCube envMap;      
varying vec3 normal; 
varying vec3 vertex;
float eta = .77; // ratio of IORs (n2/n1 in Snell's law)
void main() {
    vec3 n = normalize(normal);     
    vec3 e = normalize(vertex);
    vec3 t = gl_ModelViewMatrixInverse * vec4(refract(e, n, eta), 0.0);
    gl_FragColor = vec4(texture(envMap, t).rgb, 1.0);
}
```

However, this is not very realistic. Recall the Fresnel equations described above. When light hits an interface between two media, some is reflected and some is refracted. Furthermore, different wavelengths of light get refracted different amounts.


Open `glass.vert` and `glass.frag`. The vertex shader is already complete, so you just need to fill in the fragment shader. You will need Schlick's approximation to determine how much light is reflected and how much is refracted. The ratio of reflected to refracted light should be the Fresnel term, like in the metal shader.

 Then, we need to refract light by different amounts for each color channel (R, G, and B)—each wavelength of light interacts with the material slightly differently. To do this, change your shader to calculate three refraction vectors (one for each color channel), each of which is refracted by a slightly different amount (use the values in the `uniform vec3 eta` declared for you in the stencil code).

 To summarize, here are the high-level steps which should be performed by `glass.frag`:
1. Sample the cube map to determine the reflection color.
2. For each of the R, G, and B channels, determine the direction from which refracted light originated.
    - For example use `refract(cameraToVertex, normal, eta.r)` to get the refracted direction for `R`
3. Use these direction vectors to respectively sample the `R`, `G`, and `B` channels in the environment map.
    - Don’t forget to convert the vectors to world space first
4. Calculate the reflectance `F` by Schlick's approximation
5. Get the final color by blending the refraction color and the reflection color
    - Use the formula `refractionColor(1 - F) + reflectionColor(F)`
    - You might consider using GLSL's `mix` function for this

Try changing around the `r0` value to see the mix of reflectance and refraction change. Also try messing around with the eta values to see some fun color-dependent refraction effects! The final product should look like this:

<img src="https://i.imgur.com/krasMGm.png">

## Checkoff

Once you see both the glass and metal shaders working, ask a TA to get checked off for lab. Now you have the basis to implement shaders for other reflective/refractive materials, and you can reuse these shaders in the final project, too!

Be prepared to answer one or more of the following:
- What is the purpose of Schlick's approximation? What are the coefficients we approximate with it?
- What is environment mapping and how do we use it in our material shaders?
- How does the Cook-Torrance model differ from the Phong model?
- What is the difference in the implementation of the metal and glass shaders to allow the object to look like it has different material properties?


## Food for thought
- Review the Phong reflection model: http://en.wikipedia.org/wiki/Phong_shading#Phong_reflection_model
- More about the Blinn-Phong shading model; https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model
- Learn more about the Fresnel equations: http://en.wikipedia.org/wiki/Fresnel_equations
- Original Cook-Torrance Paper: http://inst.cs.berkeley.edu/~cs294-13/fa09/lectures/cookpaper.pdf
- OpenGL FAQ: https://www.khronos.org/opengl/wiki/FAQ