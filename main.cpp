#include <iostream>
#include <SFML/Graphics.hpp>
#include "mesh.h"
#include "matrix.h"
#include "consts.h"
#include "math.h"

///		<summary>
///		The (0, 0) point is set to be in the center of screen!
///		</summary>
///
///		Global variables
/// 
Mesh cubeMesh; // temporary
Matrix projectionMatrix(4, 4);
sf::Clock* c = nullptr; // clock from the program initialization

/// <summary>
/// Recalculates projection matrix for a view of requested width and height
/// </summary>
/// <param name="windowWidth">View width</param>
/// <param name="windowHeight">View height</param>
void calculateProjectionMatrix(int windowWidth, int windowHeight)
{
	float aspectRatio = (float)windowHeight / (float)windowWidth;
	float radFov = FOV / 180.0f * PI;
	float invTg = 1.0f / tanf(radFov * 0.5f);
	float zScaleFactor = Z_FAR_PLANE / (Z_FAR_PLANE - Z_NEAR_PLANE);

	projectionMatrix[0][0] = aspectRatio * invTg;
	projectionMatrix[1][1] = invTg;
	projectionMatrix[2][2] = zScaleFactor;
	projectionMatrix[2][3] = 1.0f;
	projectionMatrix[3][2] = -1.0f * zScaleFactor * Z_NEAR_PLANE;

#ifndef NDEBUG
	projectionMatrix.printMatrix();
#endif // NDEBUG
}

/// <summary>
/// Returns a vector projected on 2D view (only x and y coordinates are valid). 
/// Uses global variable projectionMatrix.
/// </summary>
/// <param name="v">Vector that we project</param>
/// <returns>Projected vector</returns>
Vector3d projectVector(Vector3d& v)
{
	Matrix temp(1, 4);
	temp[0][0] = v.x;
	temp[0][1] = v.y;
	temp[0][2] = v.z;
	temp[0][3] = 1.0f;
	
	temp = temp * projectionMatrix; // resulting matrix will be of the same size

	Vector3d res(temp[0][0], temp[0][1], temp[0][2]);
	if (temp[0][3] != 0.0f)
	{
		res.x /= temp[0][3];
		res.y /= temp[0][3];
	}
	return res;
}


/// <summary>
/// Creates a 3d space rotation matrix which produces rotation by corresponding angles
/// </summary>
/// <param name="xAngle">X rotation angle in radians</param>
/// <param name="yAngle">Y rotation angle in radians</param>
/// <param name="zAngle">Z rotation angle in radians</param>
/// <returns>Resulting rotation matrix (3x3)</returns>
Matrix createRotationMatrix(float xAngle, float yAngle, float zAngle)
{
	Matrix rotX(3, 3);
	rotX[0][0] = 1.0f;
	rotX[1][1] = cosf(xAngle);
	rotX[1][2] = -sinf(xAngle);
	rotX[2][1] = sinf(xAngle);
	rotX[2][2] = cosf(xAngle);

	Matrix rotY(3, 3);
	rotY[0][0] = cosf(xAngle);
	rotY[0][2] = sinf(xAngle);
	rotY[1][1] = 1.0f;
	rotY[2][0] = -sinf(xAngle);
	rotY[2][2] = cosf(xAngle);

	Matrix rotZ(3, 3);
	rotZ[0][0] = cosf(xAngle);
	rotZ[1][1] = cosf(xAngle);
	rotZ[0][1] = -sinf(xAngle);
	rotZ[1][0] = sinf(xAngle);
	rotZ[2][2] = 1.0f;

	return rotX * rotY * rotZ;
}

/// <summary>
/// Draws a line from v1 to v2 (in 2D space) using RenderWindow.
/// </summary>
/// <param name="window">Renderer</param>
/// <param name="v1">Line beginning point (2D)</param>
/// <param name="v2">Line end point (2D)</param>
/// <param name="color">Line color</param>
void DrawLine(sf::RenderWindow& window, Vector3d v1, Vector3d v2, sf::Color color)
{ 
	// Center of the window is the (0, 0) point so offset vectors by half the window size
	unsigned int offsetX = window.getSize().x / 2, offsetY = window.getSize().y / 2;
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(v1.x + offsetX, v1.y + offsetY)),
		sf::Vertex(sf::Vector2f(v2.x + offsetX, v2.y + offsetY))
	};
	line->color = color;
	window.draw(line, 2, sf::Lines);
}

/// <summary>
/// Draws triangle wireframe.
/// </summary>
/// <param name="window">Renderer</param>
/// <param name="projectedTriangle">The drawn triangle</param>
void DrawTriangle(sf::RenderWindow& window, Triangle projectedTriangle)
{
	sf::Color color = sf::Color::White;
	DrawLine(window, projectedTriangle[0], projectedTriangle[1], color);
	DrawLine(window, projectedTriangle[1], projectedTriangle[2], color);
	DrawLine(window, projectedTriangle[0], projectedTriangle[2], color);
}

/// <summary>
/// Draws a filled triangle.
/// </summary>
/// <param name="window">Renderer</param>
/// <param name="projectedTriangle">The drawn triangle</param>
void FillTriangle(sf::RenderWindow& window, Triangle projectedTriangle)
{
	sf::ConvexShape triangle; 
	triangle.setPointCount(3);
	triangle.setFillColor(sf::Color::Green);
	triangle.setOutlineColor(sf::Color::Transparent);

	unsigned int offsetX = window.getSize().x / 2, offsetY = window.getSize().y / 2;
	triangle.setPoint(0, sf::Vector2f(projectedTriangle[0].x + offsetX, 
			projectedTriangle[0].y + offsetY));
	triangle.setPoint(1, sf::Vector2f(projectedTriangle[1].x + offsetX, 
			projectedTriangle[1].y + offsetY));
	triangle.setPoint(2, sf::Vector2f(projectedTriangle[2].x + offsetX, 
			projectedTriangle[2].y + offsetY));

	window.draw(triangle);
}
 
/// <summary>
/// Draws a mesh of triangles
/// </summary>
/// <param name="window"></param>
/// <param name="mesh"></param>
/// <param name="wireframed">If true, additionally draws a wireframe of a triangle</param>
void DrawMesh(sf::RenderWindow& window, Mesh mesh, bool wireframed = false)
{
	for (int i = 0; i < mesh.tris.size(); i++)
	{
		// Project triangle, then draw it
		Triangle projectedTriangle = mesh.tris[i];
		projectedTriangle.translate(-0.5f, -0.5f, -0.5f); // temporary, for test cube
		if (c) // Rotation // temporary, for test cube
		{
			float fElapsedTime = c->getElapsedTime().asSeconds();
			Matrix rotMat = createRotationMatrix(fElapsedTime, 0.0f, fElapsedTime * 0.5f);
			//rotMat.printMatrix();
			for (int i = 0; i < 3; i++)
				projectedTriangle[i] = projectedTriangle[i] * rotMat;
		}
		projectedTriangle.translate(0.0f, 0.0f, 2.0f); // temporary, for test cube

		for (int i = 0; i < 3; i++)
			projectedTriangle[i] = projectVector(projectedTriangle[i]);

		// Scale to the size of a window
		projectedTriangle.scaleXY(0.5f * window.getSize().x, 0.5f * window.getSize().y);
		// Draw Triangle
		FillTriangle(window, projectedTriangle);
		
		// Draw wireframe
		DrawTriangle(window, projectedTriangle);
	}
}

/// <summary>
/// Initializes everything in the beginning of the program.
/// </summary>
void init()
{
	
	std::ios::sync_with_stdio(0);

	cubeMesh.tris =
	{

		// SOUTH FACE
		{ {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f} },
		{ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },

		// EAST FACE                                                      
		{ {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} },
		{ {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f} },

		// NORTH FACE                                                     
		{ {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f} },
		{ {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },

		// WEST FACE                                                      
		{ {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
		{ {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} },

		// TOP FACE                                                       
		{ {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} },
		{ {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f} },

		// BOTTOM FACE                                                    
		{ {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} },
		{ {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} }

	};

	calculateProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

	c = new sf::Clock();

}

int main()
{
	init();
	
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), 
		"3DRenderer", sf::Style::Default, settings);

#ifdef NDEBUG
	window.setFramerateLimit(60);
#else
	window.setFramerateLimit(30);
#endif // NDEBUG

	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::Resized)
			{
				// recalculate projection matrix
				calculateProjectionMatrix(window.getSize().x, window.getSize().y);
				
				// reset the window view
				sf::FloatRect newViewRect = sf::FloatRect(0, 0, window.getSize().x, window.getSize().y);
				window.setView(sf::View(newViewRect));
			}
		}
        window.clear();

		DrawMesh(window, cubeMesh);
        
		window.display();
    }

    return 0;
}