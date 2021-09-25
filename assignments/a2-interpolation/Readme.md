# Readme



Gradient


![gradient](https://user-images.githubusercontent.com/60118869/134764276-79802efc-6ee3-4acc-8d32-4069284140d6.gif)

A gif above shows an N X N window which represents a 2D gradient with colors interpolated based on x and y directions. The color is determined based on the position of a cube.


Draw Curve 


![drawcurve](https://user-images.githubusercontent.com/60118869/134764907-06ae6fbc-67af-4f10-b353-334ba8646131.gif)


The gif above shows that when the user presses the "1" key, the curve gets computed and drawn using Bernstein polynomial (in blue) and when the user presses "2" key, the same curve gets computed using De Casteljau’s algorithm (in red). Both methods give the same curve.


Particle Cubic



![particlecubic](https://user-images.githubusercontent.com/60118869/134765036-14fe90d2-0826-4319-b7fb-2290fcd934e9.gif)


The give above shows the movement of the particle along the curve. The particle takes 5 seconds to travel the curve and returns to the beginning of the curve when it gets to the end.




Single Curve

![curves](https://user-images.githubusercontent.com/60118869/134713438-85142106-9d15-42c6-948a-592b39e6855f.gif)

The above gif shows the single curve step of the screensaver when the intermediate interpolated curve is drawn once per frame. Both the begining and the end curve are drawn too with the colors interpolated between them. Once 1 second passes, the new curve gets generated which becomes a new interpolation target.


Trail effect



https://user-images.githubusercontent.com/60118869/134766593-354e7f4b-c829-483c-808e-a1b6617ec4be.mp4


In the gif above, the algorithm described before is extended to include drawing intermediate curves at each step (trails). When the number of curves drawn reaches the limit of fifty, the curve drawn the earliest gets removed. The drawing of each curve is spaced by a certain amount of time so that they don't look clustered together.




Unique Scene

![unique](https://user-images.githubusercontent.com/60118869/134763867-be9338a0-42c9-4710-a8ac-5267fa4709bb.gif)

The above gif shows a scene where the control points of the curves get animated using a sine. This allows for smooth movement of the curves in the same rhytm. The colors of the curves are interpolated and the collection of curves represents a gradient.


