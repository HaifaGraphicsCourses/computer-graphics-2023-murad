## cout vertices and faces

![Microsoft Visual Studio Debug Console 11_24_2022 8_25_21 PM](https://user-images.githubusercontent.com/115185916/203847618-ec2138c4-1841-4432-b5c0-55165b605e4c.png)

## Scale and translate the vertices such that they will fit inside your window:
every mesh have different vertices range so we cant use the same scale for every mesh for example some meshes the range is [-1,1] while other meshes have vertices in the range [200, -200] and using the same scale will either be too big or too small. to find a suitable scale we itarte over the vertices and find the max using std::max function and compare the current max and other vertices then we go over every vertix again and scale it by 200/max this allows us to scale every mesh by 200 and fit in our window after that we translate it to be in the middle of the window so for the x coordinate we translate it to (screeen width)/2 and the y coordinate by (screen height)/2.

## draw all the lines using the DrawLine function.
![Mesh Viewer 11_25_2022 4_18_39 PM](https://user-images.githubusercontent.com/115185916/204026923-c9ea4a30-c2a1-4a96-9902-434d33d35bef.png)
![Mesh Viewer 11_25_2022 4_17_32 PM](https://user-images.githubusercontent.com/115185916/204026937-c1d49c50-c046-4cce-be71-235b7241b412.png)
![Mesh Viewer 11_25_2022 6_36_52 PM](https://user-images.githubusercontent.com/115185916/204026960-e7b63eb0-7954-4ce3-bb20-cdcadfe58097.png)
![Mesh Viewer 11_25_2022 6_37_24 PM](https://user-images.githubusercontent.com/115185916/204026970-da66e3fd-1ab5-4b84-9112-13650d739a8d.png)
![Mesh Viewer 11_25_2022 6_42_03 PM](https://user-images.githubusercontent.com/115185916/204027536-5aaeca0d-f701-48e5-a770-e9e4dcc40871.png)

## Create GUI items to specify the local and world transformations that should be applied to the mode:
![Mesh Viewer 11_26_2022 6_26_28 PM](https://user-images.githubusercontent.com/115185916/204098849-0739a495-33a8-4bb5-81f8-e49ee9b48bac.png)
![Mesh Viewer 11_26_2022 6_26_11 PM](https://user-images.githubusercontent.com/115185916/204098850-94ebd185-2090-44cb-bbaa-e800387c619a.png)
![Mesh Viewer 11_26_2022 6_26_19 PM](https://user-images.githubusercontent.com/115185916/204098851-1c9ffad4-c0c2-4654-a364-f45528e95afc.png)
a simple UI there are two windows one for the local transformations and one for the world transfoermations each window has 3 choises scale, translate and rotate.
the UI controls controls the value saved in the mesh itself wich allows us to apply the transformations in each frame independently in real time and see the transformations happening. to apply the transformations we create a glm::vec4 with wich has (v.x, v.y, v.z, 1) for every vertice and 6 4x4 matrices for the wolrd and local transformations, and apply them this way (WorldTranslate * WorldRotate * WorldScale) * (LocalTranslate * LocalRotate * localScale) * ourVector for every vertice. for some reason if we apply the scale and translate to vertices such that they will fit inside our window there will be a bug so the only way it works right is to apply the scale with the local scale and the translate with the world translate.

## Show two screenshots comparing thedifference between translating in the model frame and then rotating in the world frame,vs. translating in the world frame and then rotating in the local frame.

local translate and then world rotate:
![Mesh Viewer 11_28_2022 5_33_53 PM](https://user-images.githubusercontent.com/115185916/204318021-2332e471-841c-4a7c-8e84-34edeb73165f.png)

translate in the world frame and then rotate in in local frame:
![Mesh Viewer 11_28_2022 5_34_42 PM](https://user-images.githubusercontent.com/115185916/204318256-82b30239-d684-4b71-b5ed-3056f0fc2a7d.png)

## Enable loading several models, switching between the active models and transforming each independently.

![Mesh Viewer 11_29_2022 12_52_56 AM](https://user-images.githubusercontent.com/115185916/204398280-82243766-6466-49c2-ad4b-d760227e6175.png)
![Mesh Viewer 11_29_2022 12_54_59 AM](https://user-images.githubusercontent.com/115185916/204398284-e92b72d2-d412-462e-9dd2-fc8331c48b2f.png)
![Mesh Viewer 11_29_2022 12_55_13 AM](https://user-images.githubusercontent.com/115185916/204398288-65ebac95-835b-45b7-9ce3-631bd89d3c47.png)
