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
![Mesh Viewer 12_17_2022 7_55_01 PM](https://user-images.githubusercontent.com/115185916/208255040-2021496b-f4ea-47c3-8a97-64685109787c.png)
![Mesh Viewer 12_17_2022 7_52_27 PM](https://user-images.githubusercontent.com/115185916/208255041-1a2fc7db-e5ab-4d7a-b77c-7c0a13014dc6.png)




