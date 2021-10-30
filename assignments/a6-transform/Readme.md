# Readme

1. Slerp

Test output:

<img width="678" alt="Capture_2" src="https://user-images.githubusercontent.com/60118869/139561156-51bfd1fa-40be-4f95-8270-cc486cd2480a.PNG">

Teapot rotation:

![slerp](https://user-images.githubusercontent.com/60118869/139561246-50e185c6-e314-4bb2-bfee-26e871201fe8.gif)

We can see that slerp implementation passed all the tests and the rotation is as expected.


2. Stacking boxes


![slerp](https://user-images.githubusercontent.com/60118869/139561266-0bffbd63-61e8-4878-9795-9efa7f9b0754.gif)

We can see that boxes are at their expected positions. 

Positions of boxes relative to other boxes output:

<img width="477" alt="Capture_3" src="https://user-images.githubusercontent.com/60118869/139561289-fc87ac9f-5f35-44f5-a48e-1ec48d9863cd.PNG">

3. Skeleton

![skeleton](https://user-images.githubusercontent.com/60118869/139561309-225bde20-215d-4b71-a475-303444ff179c.gif)

We can see that the output obtained as a result of reading the BVH file is as expected.

4. Tentacle


![tentacle](https://user-images.githubusercontent.com/60118869/139561356-24f8e11e-a1e3-4dbd-a731-f6e950b541ad.gif)

We can see a moving tentacle with 5 joints where each joint is at distance of 50 units from the previous one. The joint of each tentacle is rotated around its local Z-axis using a quaternion and the angle of rotation is computed using a sine. The angle of rotation is offset for each joint.

5. Unique (A waving character with a "waving" flower)

![unique](https://user-images.githubusercontent.com/60118869/139561440-de763a24-7c20-4dc1-8fa8-551f26c87345.gif)

We can see a character waving with one of its hand limbs. The motion is achieved with quaternion rotation along the local X-axis and the angle of rotation is computed using sine. There is also a flower with a stem which is rotating smoothly never touching the ground creating the "greeting atmosphere". The petals have random colors which change based on position of rotation. There is also another flower drawn using rose curve shape applied to two joints. 


