#include <iostream>
#include <SFML/Graphics.hpp>
#include "mesh.h"
#include "matrix.h"
#include "consts.h"
#include "math.h"

/// <summary>
/// The (0, 0) point is set to be in the center of screen!
/// </summary>

Mesh cubeMesh;

Matrix projectionMatrix(4, 4);

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

	projectionMatrix.printMatrix();
}

Vector3d projectVector(Vector3d& v)
{
	Matrix temp(1, 4);
	temp[0][0] = v.x;
	temp[0][1] = v.y;
	temp[0][2] = v.z;
	temp[0][3] = 1.0f;
	
	temp = Matrix(temp * projectionMatrix);

	Vector3d res(temp[0][0], temp[0][1], temp[0][2]);
	if (temp[0][3] != 0.0f)
	{
		res.x /= temp[0][3];
		res.y /= temp[0][3];
	}
	return res;
}

sf::Clock* c = nullptr;

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

	//std::cout << "rotX:\n";
	//rotX.printMatrix();

	Matrix rotY(3, 3);
	rotY[0][0] = cosf(xAngle);
	rotY[0][2] = sinf(xAngle);
	rotY[1][1] = 1.0f;
	rotY[2][0] = -sinf(xAngle);
	rotY[2][2] = cosf(xAngle);

	//std::cout << "rotY:\n";
	//rotY.printMatrix();

	Matrix rotZ(3, 3);
	rotZ[0][0] = cosf(xAngle);
	rotZ[1][1] = cosf(xAngle);
	rotZ[0][1] = -sinf(xAngle);
	rotZ[1][0] = sinf(xAngle);
	rotZ[2][2] = 1.0f;

	//std::cout << "rotZ:\n";
	//rotZ.printMatrix();

	return rotX * rotY * rotZ;
}

void DrawLine(sf::RenderWindow& window, Vector3d v1, Vector3d v2, sf::Color color)
{ 
	unsigned int offsetX = window.getSize().x / 2, offsetY = window.getSize().y / 2;
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(v1.x + offsetX, v1.y + offsetY)),
		sf::Vertex(sf::Vector2f(v2.x + offsetX, v2.y + offsetY))
		/*sf::Vertex(sf::Vector2f(v1.x, v1.y)),
		sf::Vertex(sf::Vector2f(v2.x, v2.y))*/
	};
	line->color = color;
	window.draw(line, 2, sf::Lines);
}

void DrawTriangle(sf::RenderWindow& window, Triangle projectedTriangle)
{
	sf::Color color = sf::Color::Red;
	// Все 3 вызова рисуют одно и то же
	DrawLine(window, projectedTriangle[0], projectedTriangle[1], color);
	DrawLine(window, projectedTriangle[1], projectedTriangle[2], color);
	DrawLine(window, projectedTriangle[0], projectedTriangle[2], color);
}

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

void DrawMesh(sf::RenderWindow& window, Mesh mesh)
{
	for (int i = 0; i < mesh.tris.size(); i++)
	{
		// Project triangle, then draw it
		Triangle projectedTriangle = mesh.tris[i];
		projectedTriangle.translate(-0.5f, -0.5f, -0.5f);
		if (c) // Rotation
		{
			float fElapsedTime = c->getElapsedTime().asSeconds();
			Matrix rotMat = createRotationMatrix(fElapsedTime, 0.0f, fElapsedTime * 0.5f);
			//rotMat.printMatrix();
			for (int i = 0; i < 3; i++)
				projectedTriangle.vertices[i] = projectedTriangle.vertices[i] * rotMat;
		}
		//projectedTriangle[0].printVector();
		projectedTriangle.scaleXY(0.5f * window.getSize().x, 0.5f * window.getSize().y);
		projectedTriangle.translate(0.0f, 0.0f, 3.0f);

		for (int i = 0; i < 3; i++)
			projectedTriangle.vertices[i] = projectVector(projectedTriangle.vertices[i]);

		// Draw Triangle
		//FillTriangle(window, projectedTriangle);

		// Draw wireframe
		DrawTriangle(window, projectedTriangle);
	}
}

void init()
{
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
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "3DRenderer", sf::Style::Default, settings);
	window.setFramerateLimit(60);
	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::Resized)
				calculateProjectionMatrix(window.getSize().x, window.getSize().y);
        }

        window.clear();

		DrawMesh(window, cubeMesh);
        
		window.display();
    }

    return 0;
}