# C++ Ray Tracer

A C++ ray tracer with the focus on rendering realistic images at low CPU cost regardless of the amount of dynamic lighting in the scenery. 
Example:
![sample image](https://github.com/jawedib/raytracer/blob/master/sampleImage.jpg?raw=true)



This project started off as a school assignment with the requirement being creating a ray tracer that doesn't involve any reflections:

![course requirement](https://github.com/jawedib/raytracer/blob/master/progress1.jpg?raw=true)




Over the winter break I decided to take it to the next level using my own intuition, course notes and being mentally prepared to fail a lot. And indeed there were issues but I at least now I got to where I am now while understanding it inside out. The first success came when I was able to get the reflections work in a sense that objects took on the color of other objects around them with proper depth information provided they are set as shiny objects:

![first success](https://github.com/jawedib/raytracer/blob/master/progress2.jpg?raw=true)



As of 30th April 2019, adding a light to any scene costs a fraction of a second of processing time instead of doubling it. The challenge I am looking to overcome right now is for realistic light and shadow interaction as shining a new light source on a pre-existing shadow does not produce realistic results. For example:

![bug sample](https://github.com/jawedib/raytracer/blob/master/visualBugSample.jpg?raw=true)
