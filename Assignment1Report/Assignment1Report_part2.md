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
