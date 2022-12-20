## Implement a camera with an orthographic projection, and compare the result with two different view volumes

![Mesh Viewer 12_17_2022 7_26_36 PM](https://user-images.githubusercontent.com/115185916/208254011-a7dde39e-dd13-4c65-b74b-f77eb512e7a6.png)
![Mesh Viewer 12_17_2022 7_25_48 PM](https://user-images.githubusercontent.com/115185916/208254012-c367b81f-7561-43d7-a444-9cad345b7aad.png)
![Mesh Viewer 12_17_2022 7_26_13 PM](https://user-images.githubusercontent.com/115185916/208254014-9175d0d6-ca38-4d4d-81b9-4aa0cc0dbd12.png)

## Allow the user to reposition the camera in the camera frame and the world frame using
incremental changes

First function for transformations is that ,we can use the mouse for local transormations, we used the GetMouseDragDelta function that returns the delta from the initial clicking position while the mouse button is clicked or was just released. the second function is roatiting and translating the camera by using the KeyBoard buttons, using the WASD buttons for local tranformation on x and y , and the arrow keys for local rotation, the glfwGetKey function allowed us to know which keyboard button is pressed, the WS used for local translating on y ratio,the AD used for translating on x ratio, the left and right keys is used for local rotations.

results can be shown in this video:

https://user-images.githubusercontent.com/115185916/208254439-b8297d79-be31-4ee8-8ba2-74f377cb2f34.mp4

## Allow the user to set the view volume automatically based on the window’s size, and show the result on two window sizes

this is a 1280 * 720 window:

![Screenshot (2)](https://user-images.githubusercontent.com/115185916/208254763-1c4cce94-9971-47da-93ae-4438d69a972f.png)

and this is a 2560 * 1440 window:

![Screenshot (3)](https://user-images.githubusercontent.com/115185916/208254783-46627dd5-1971-43ef-963d-e3b2ffca8960.png)


## Implement a feature in the renderer that draws the axes of the model and the world frame. Show the difference between transforming a model in the world frame and in the model frame while its axes are visible

note: red axis are world axis and blue axis are model axis
![Mesh Viewer 12_17_2022 7_53_01 PM](https://user-images.githubusercontent.com/115185916/208255036-4d3aaa2f-0395-4d75-b811-e6005f382fa7.png)
![Mesh Viewer 12_17_2022 7_53_58 PM](https://user-images.githubusercontent.com/115185916/208255037-ca588dc6-a06a-47d4-ad24-27a884c9bd78.png)
![Mesh Viewer 12_17_2022 7_54_33 PM](https://user-images.githubusercontent.com/115185916/208255038-c245e1a3-4472-4bfe-a701-3b580613a285.png)
![Mesh Viewer 12_17_2022 7_54_49 PM](https://user-images.githubusercontent.com/115185916/208255039-21402a94-40ee-45da-bfb9-3becf55eebb7.png)

## Load a different mesh and pick two non-commuting transformations T1, T2. Compare the results of applying T1 in model frame and T2 in world frame vs .T1 in world frame and T2 in world frame.

first picture is translating in world frame and rotating in local frame which moves the axis of the model and then rotating the model in the local frame keeps the axis as they were but rotates the model. the second picture is rotating the model in the world frame which also rotates the axis and then translating the model in the local frame moves it along its axis which are rotated.
![Mesh Viewer 12_17_2022 9_25_51 PM](https://user-images.githubusercontent.com/115185916/208263050-45014ca4-457a-4a76-a79c-3947d76a578f.png)
![Mesh Viewer 12_17_2022 9_24_30 PM](https://user-images.githubusercontent.com/115185916/208263051-78007fbb-1d1d-4a39-9834-6333368b7dec.png)



## Implement and display the bounding box of the object, in local and world coordinates. Demonstrate the differences between them.

a bounding box in local coordinates moves exactly like the model especially the rotation and never changes shape while the bounding box in world coordinates when rotating changes to the new max and min coordinates

https://user-images.githubusercontent.com/115185916/208265622-f7b58484-2c61-4a15-ade5-2a45af99e0df.mp4

## Compute and display the face normals and vertex normals

every vertex normal has more than one normal, one for every face we add the normal to the vertex coordinates and then scale it to draw a line between the vertex and the new created normal, while face normals are different firs we find the normal using cross(v2-v1, v3-v1) and then normalize it then we find the middle of each edge in the face and do the same here like we did in the vertex normal we add them then scale...
![Mesh Viewer 12_18_2022 9_59_20 PM](https://user-images.githubusercontent.com/115185916/208316858-0e63047c-8698-4e09-ab5f-c5ce21bb34d4.png)
![Mesh Viewer 12_18_2022 9_58_53 PM](https://user-images.githubusercontent.com/115185916/208316859-404773e2-db66-4533-9ec7-ac4e0c9732dc.png)
![Mesh Viewer 12_18_2022 9_59_13 PM](https://user-images.githubusercontent.com/115185916/208316861-83b973e4-6379-4d7f-9f23-64176d16ff03.png)

## implement a perspective projection and compare the orthographic projection vs. the perspective projection
the diffrence is clearly visible between orthographic projection and perspective projection, thanks to glm::ortho and glm::frustrum, we compared the diffrences by this functions, the glm::ortho or the orthographic projection is stable and dont change by the camera view, dont change by the translations in z ratio otherwise the perspictive projection is senstive to translating in all directions,its small when its far from the camera line, and getting bigger while getting closer to the camera center.

## how the difference between changing the zoom and moving closer or farther from the model
the diffrence is clearly visible, the more the mesh is near the bigger the mesh is:
![image](https://user-images.githubusercontent.com/116717525/208476741-4c4768fe-540f-47fe-b6ca-1e64c972c13b.png)
![image](https://user-images.githubusercontent.com/116717525/208476879-6dace405-c3fe-4c22-bfa1-ecb9a724aedf.png)
![image](https://user-images.githubusercontent.com/116717525/208477002-5922ac12-4597-45e3-b7e0-7c72b2f8dfc4.png)

## how the difference between changing the zoom (by changing the size of the frustum) and moving closer or farther from the model.
![Mesh Viewer 12_19_2022 11_38_42 PM](https://user-images.githubusercontent.com/115185916/208529604-ae45e05b-77a5-47f2-ab4f-16aa711f6d8f.png)
![Mesh Viewer 12_19_2022 11_41_00 PM](https://user-images.githubusercontent.com/115185916/208529608-6b480162-e0fa-4c5b-b9e6-db33127414fe.png)

## Put at least two screenshots showing multiple models and cameras on screen.
![image](https://user-images.githubusercontent.com/116717525/208680140-32b8945e-0d4d-42fa-884f-e3f0df6e53f7.png)
![image](https://user-images.githubusercontent.com/116717525/208683499-6095a2fe-099f-42ba-b8f6-6476f28d31c4.png)

## Put a screenshot showing as much of your GUI as possible.
![image](https://user-images.githubusercontent.com/116717525/208709492-ac6cfb82-62ad-4504-be01-dfed228b779d.png)
![image](https://user-images.githubusercontent.com/116717525/208709591-0b869367-47a3-4e5a-a106-53036e9d75e2.png)
![image](https://user-images.githubusercontent.com/116717525/208709697-bd0e6f08-c380-45c0-9123-06d96a9d578b.png)


