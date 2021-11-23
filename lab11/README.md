# Lab 11 - Dielectric Materials

In this lab, we’ll cover how we simulate a special type of material called a dielectric in either real-time rendering or raytracing (you’ll choose one to implement!)

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

<img src="https://render.githubusercontent.com/render/math?math=\eta = \sqrt{\varepsilon_r}" justify="center" width="200px">

## Snell’s Law
We can relate the indices of refraction of two materials to how light will refract at their interface using Snell’s Law.

<img src="https://render.githubusercontent.com/render/math?math=\frac{\sin{\theta_1}}{\sin{\theta_2}} = \frac{\eta_2}{\eta_1}" justify="center" width="200px">


We can calculate the refraction of light at an interface if we know the indices of refraction of the two incident materials and the angle of the ray of light entering the interface.

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/3f/Snells_law2.svg/800px-Snells_law2.svg.png" width="200px" justify="center">

Now we can bend light! The amount it bends relates back to the dielectric constant of the medium it exits and the medium it enters. 

We’re not quite done determining how to simulate glassy surfaces. If you’ve ever looked at a snowglobe or another glass object, you’ve probably also seen some light reflected off the surface. We’ll need a bit more physics to think about this phenomenon.

## Fresnel Reflectance
Assuming the material we’re simulating does not absorb light, all light that hits the surface of the material must either be transmitted or reflected due to conservation of energy. But how do we know how much light gets transmitted and how much gets reflected? 

It depends on the _polarization_ of the light, which refers to whether the light wave lies in the plane of the incident surface (P-polarization) or is perpendicular to the incident surface (S-polarization). Fresnel’s Law describes the reflectance, or percentage of incident light reflected, for each type of polarized light.

We include the original Fresnel equations below - they’ll come in handy soon!

<img src="https://render.githubusercontent.com/render/math?math=R_s = \left|\frac{\eta_1\cos\theta_i - \eta_2\cos\theta_t}{\eta_1\cos\theta_i %2B \eta_2\cos\theta_t}\right|^2" justify="center" width="200px">

<img src="https://render.githubusercontent.com/render/math?math=R_p = \left|\frac{\eta_2\cos\theta_i - \eta_1\cos\theta_t}{\eta_2\cos\theta_i %2B \eta_1\cos\theta_t}\right|^2" justify="center" width="200px">

In graphics, it’s common to assume that light is unpolarized, which means that we can calculate a single reflectance value for a given ray of light. Assuming light is unpolarized means that the overall percentage of light reflected rather than transmitted is 

<img src="https://render.githubusercontent.com/render/math?math=R_F = (R_s %2b R_p) / 2" justify="center" width="200px">

Notice that this value depends on the angle of incidence of the ray. As with the simulation of many physical phenomena in computer graphics, we’re going to leverage a good approximation of Fresnel reflectance to reduce the number of calculations we need to perform. 

## Schlick’s Approximation
Schlick (1994) introduced an approximation of Fresnel reflectance that is widely used in real-time rendering (and in our case, raytracing as well!)


<img src="https://render.githubusercontent.com/render/math?math=R_F = R_0 %2b (1-R_0)(1-\cos\theta)^5" justify="center" width="200px">

where 

<img src="https://render.githubusercontent.com/render/math?math=R_0 = \left(\frac{\eta_1 - \eta_2}{\eta_1 %2b \eta_2}\right)^2" justify="center" width="200px">

<img src="https://render.githubusercontent.com/render/math?math=\cos \theta = n \cdot (-\omega)" justify="center" width="200px">

for <img src="https://render.githubusercontent.com/render/math?math=n"> the surface normal and <img src="https://render.githubusercontent.com/render/math?math=\omega">  the vector from the light source to the surface.

Because <img src="https://render.githubusercontent.com/render/math?math=R_0"> doesn’t depend on the angle at which the light hits the surface, we can calculate it once per surface and store it.


We can now calculate both the percentage of light reflected off of a surface and the direction that light will travel when transmitted through the surface. This is everything we need to know to simulate a glassy surface! Almost :) 

One last important note: we weren’t entirely honest earlier when we said there’s a single index of refraction for a material. The index of refraction varies based on the wavelength of the light incident to the surface. **This will only be relevant in the real-time implementation option. For the raytraced implementation, we will assume the index of refraction is independent of the wavelength of light.**

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

<img src="https://render.githubusercontent.com/render/math?math=R_s(\omega, \omega_r) = \frac{F(\omega)}{\pi}\cdot\frac{DG}{\pi(n\cdot(-\omega))(n\cdot\omega_r)}" justify="center" width="200px">

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