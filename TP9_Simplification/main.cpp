#include "main.h"

void printCommandes()
{
	printf("\n=== Commandes ===\n\n");
	printf(" A / Z : zoom\n");
	printf(" F : mode fill\n");
	printf(" T : mode texture\n");
	printf(" R : changer de maillage\n");
	printf(" S : export mesh\n");
	printf(" D : supprimer une face\n");
	printf(" Espace : augmenter l'angle de rotation de 10 deg\n");
	printf(" Echap : quitter\n");
}

int main(int argc, char **argv)
{
	// initialisation  des paramètres de GLUT en fonction
	// des arguments sur la ligne de commande
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// définition et création de la fenêtre graphique
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(500, 400);
	glutCreateWindow("Simplification de maillage");

	// initialisation de OpenGL et de la scène
	initGL();
	init_scene();

	// choix des procédures de callback pour 
	// le tracé graphique
	glutDisplayFunc(&window_display);
	// le redimensionnement de la fenêtre
	glutReshapeFunc(&window_reshape);
	// la gestion des événements clavier
	glutKeyboardFunc(&window_key);
	glutIdleFunc(&window_idle);
	glutMouseFunc(&window_mouse_click);
	glutPassiveMotionFunc(&window_mouse);
	// la boucle prinicipale de gestion des événements utilisateur
	glutMainLoop();

	return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque

GLvoid initGL()
{
	glClearColor(RED, GREEN, BLUE, ALPHA);
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

	/* enable lighting */
	glEnable(GL_LIGHTING);
	///* enable lights (note: uses default values) */
	glEnable(GL_LIGHT0);

	//// Set lighting intensity and color
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void init_scene()
{
	mode_rendu = 0;
	mode_fill = true;
	mode_texture = true;
	dist = -7.0;
	printf("Mesh : %s\n", changeMesh());
	printCommandes();
	std::vector<float> colors = mesh.getColors();
	std::vector<float> vertices = mesh.getVertices();
	//std::vector<Vec3<float>> normales = mesh.getNormales();
	std::vector<unsigned int> indices = mesh.getIndices();

	//// Génération des buffers
	//glGenBuffers(3, buffers);

	//buffers = new GLuint[3];

	//buffers[0] = 1;
	//buffers[1] = 2;
	//buffers[2] = 3;

	//// Buffer d'informations de vertex
	//glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

	//// Buffer d'indices
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	//// Buffer de textures
	//glBindBuffer(GL_TEXTURE_BUFFER, buffers[2]);
	//glBufferData(GL_TEXTURE_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);
}

// fonction de call-back pour l´affichage dans la fenêtre
GLvoid window_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_scene();

	// trace la scène graphique qui vient juste d'être définie
	glFlush();
	glutSwapBuffers();
}

// fonction de call-back pour le redimensionnement de la fenêtre
GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);

	// toutes les transformations suivantes s´appliquent au modèle de vue 
	glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des événements clavier
GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_ESC:
		exit(1);
		break;
	case 'a':
	case 'A':
		dist *= 1.1;
		break;
	case 'z':
	case 'Z':
		dist *= 0.9;
		break;
	case 'f':
	case 'F':
		mode_fill = !mode_fill;
		break;
	case 't':
	case 'T':
		mode_texture = !mode_texture;
		break;
	case 'r':
	case 'R':
		mode_rendu++;
		if (mode_rendu > 2)
			mode_rendu = 0;
		break;
	case 's':
	case 'S':
		MeshWriter::exportObj(mesh, "new.off");
		printf("Export mesh vers 'new.off'");
		break;
	case 'd':
	case 'D':
		mesh.removeFace();
		break;
	case 32: // 'Espace'
		printf("Mesh : %s\n", changeMesh());
		break;
	default:
		printf("La touche %d n´est pas active.\n", key);
		printCommandes();
		break;
	}
}

GLvoid window_mouse_click(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		look = true;
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		look = false;

	last_x = x;
	last_y = y;
}

GLvoid window_mouse(int x, int y)
{
	int dx = x - last_x;
	int dy = y - last_y;

	if (look)
	{
		rotateBy(dy * 8, 0, 0);
		rotateBy(0, dx * 8, 0);
	}

	last_x = x;
	last_y = y;
}

GLvoid window_idle()
{
	glutPostRedisplay();
}

void rotateBy(int x, int y, int z)
{
	x_rot += x;
	y_rot += y;
	z_rot += z;
}

void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, mode_fill ? GL_FILL : GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	if (mode_texture)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	// Model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3<float> centre = mesh.getCentreGravite();
	gluLookAt(0.0, 0.0, -dist,
		centre.x, centre.y, centre.z,
		0.0, 1.0, 0.0);

	glRotatef(x_rot / 16.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(y_rot / 16.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(z_rot / 16.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f, 1.0 * WIDTH / HEIGHT, 0.1f, 100.0f);

	traceMaillage();
}

std::string changeMesh()
{
	std::string m = "";

	switch (num_mesh)
	{
	case 0:
		m = "max.off";
		break;
	case 1:
		m = "new.off";
		break;
	case 2:
		m = "triceratops.off";
		break;
	case 3:
		m = "bunny.off";
		break;
	case 4:
		m = "buddha.off";
		break;
	}

	mesh = MeshWriter::importObj(m);

	if (num_mesh < 4)
		num_mesh++;
	else
		num_mesh = 0;

	printf("\nNb faces : %d\n", mesh.getFaces().size());
	printf("Nb points : %d\n", mesh.getPoints().size());
	printf("Nb segments : %d\n", mesh.getArretes().size());

	return m;
}

void traceMaillage()
{
	std::vector<Vec3<unsigned int>> faces = mesh.getFaces();
	std::vector<Vec3<float>> points = mesh.getPoints();
	std::vector<Vec3<unsigned int>> facesTextures = mesh.getFacesTextures();
	std::vector<Vec3<unsigned int>> facesNormales = mesh.getFacesNormales();
	std::vector<Vec3<float>> textures = mesh.getTextures();
	std::vector<float> colors = mesh.getColors();
	std::vector<float> vertices = mesh.getVertices();
	std::vector<unsigned int> indices = mesh.getIndices();
	std::vector<Vec3<float>> normales = mesh.getNormales();

	int a, b, c;

	switch (mode_rendu)
	{
	case 0:
		glBegin(GL_TRIANGLES);

		for (int i = 0; i < faces.size(); i++)
		{
			a = faces[i].x;
			b = faces[i].y;
			c = faces[i].z;

			glNormal3f(normales[a].x, normales[a].y, normales[a].z);
			glTexCoord3f(textures[facesTextures[i].x].x, textures[facesTextures[i].x].y, textures[facesTextures[i].x].z);
			glVertex3f(points[a].x, points[a].y, points[a].z);

			glNormal3f(normales[b].x, normales[b].y, normales[b].z);
			glTexCoord3f(textures[facesTextures[i].y].x, textures[facesTextures[i].y].y, textures[facesTextures[i].y].z);
			glVertex3f(points[b].x, points[b].y, points[b].z);

			glNormal3f(normales[c].x, normales[c].y, normales[c].z);
			glTexCoord3f(textures[facesTextures[i].z].x, textures[facesTextures[i].z].y, textures[facesTextures[i].z].z);
			glVertex3f(points[c].x, points[c].y, points[c].z);
		}

		glEnd();
		break;

	case 1:
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glNormalPointer(GL_FLOAT, 0, normales.data());
		glVertexPointer(3, GL_FLOAT, 0, vertices.data());
		glTexCoordPointer(3, GL_FLOAT, 0, colors.data());

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		break;

	case 2:
		// Utilisation des données des buffers
		//glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		//glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), NULL);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		//glTexCoordPointer(3, GL_FLOAT, 3 * sizeof(float), NULL);

		//glBindBuffer(GL_TEXTURE_BUFFER, buffers[2]);

		//// Activation d'utilisation des tableaux
		//glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);

		//// Rendu de notre géométrie
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		//glDisableClientState(GL_COLOR_ARRAY);
		//glDisableClientState(GL_VERTEX_ARRAY);
		break;
	}
}