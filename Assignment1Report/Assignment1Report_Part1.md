## mplementation of the algorithm.
	void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
	int dx, dy, p, x, y;
	int x0 = p1.x, y0 = p1.y, x1 = p2.x, y1 = p2.y; // for easier use
	if (p1.x > p2.x) // if x0 > x1 wh swap points
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	dx = x1 - x0;
	dy = y1 - y0;

	x = x0;
	y = y0;

	if (dx == 0) // for straight lines
	{
		while (y != y1)
		{
			PutPixel(x, y, color);
			if (dy > 0)
				y++;
			else
				y--;
		}
	}

	else if (abs(dy) > abs(dx)) // if y difference is greater than x difference we use this breshman algo for slopes greater than 1
	{
		p = 2 * dx - dy;
		while (y != y1)
		{
			if (p >= 0)
			{
				PutPixel(x, y, color);
				{
					x = x + 1;
					p = p + 2 * dx - 2 * abs(dy);
				}
			}
			else
			{
				PutPixel(x, y, color);
					p = p + 2 * dx;
			}
			if (dy > 0) // we move up if slope is positive and down if negative
				y = y + 1;
			else
				y = y - 1;
		}
	}

	else { // else we use this algo for slopes smaller than one
			p = 2 * abs(dy) - dx;
		while (x <= x1)
		{
			if (p >= 0)
			{
				PutPixel(x, y, color);
				if (dy < 0) // we check if slope is negative to move up or down
					y = y - 1;
				else
					y = y + 1;

				p = p + 2 * abs(dy) - 2 * dx;
			}
			else
			{
				PutPixel(x, y, color);

				p = p + 2 * abs(dy);
			}
			x = x + 1;
		}
	}
}
## pictures of the sanity check results
![Mesh Viewer 10_29_2022 9_57_13 PM](https://user-images.githubusercontent.com/115185916/198848395-0d661ec4-6e5a-4f75-adc4-fd6fd2279829.png)
![Mesh Viewer 10_29_2022 9_56_49 PM](https://user-images.githubusercontent.com/115185916/198848406-97b5b873-126a-4c80-b3c2-5a12e1b184ed.png)
## creative drawing of my choice
![Mesh Viewer 10_29_2022 10_37_46 PM](https://user-images.githubusercontent.com/115185916/198849781-c30c56e1-7d63-4614-85f6-39b640112bfb.png)
