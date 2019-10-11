#include<GL/freeglut.h>
#include<stdio.h>
#include<ctype.h>
#include<math.h>
#include<string.h>
#include<stdio.h>
#include<iostream>
#define SCREEN_SIZE_X 1500
#define SCREEN_SIZE_Y 800

char query_string[] = "Enter an Integer and Hit Enter : ";
char not_found[] = "Item Not found";
enum color{RED,GREEN,BLUE};

struct node {
	int data;
	struct node *left;
	struct node *right;
};

typedef struct node* NODE;

NODE root = NULL;
void display();
float offset = 0.0;
bool box_clicked = false;
int temp_x, temp_y, temp_item;
bool prev_search = false;

int count_digit(int n) {
	return (n == 0) ? 0 : floor(log10(n) + 1);
}

void to_string(char *s, int num) {
	int r, d;
	d = count_digit(abs(num));
	if (num == 0)
		s[0] = '0';
	if (num < 0) {
		s[0] = '-';
		d++;
	}
	s[d] = '\0';
	num = abs(num);
	while (num != 0) {
		r = num % 10;
		s[--d] = r + '0';
		num /= 10;
	}
}

void display_string(char s[], float x, float y, float z = 0.0) {
	glRasterPos3f(x, y, z);
	for (int i = 0; s[i]; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
	glFlush();
}

void clear_input_region() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2f(80, 580);
	glVertex2f(80, 650);
	glVertex2f(580, 650);
	glVertex2f(580, 580);
	glEnd();
	glFlush();
}

class binary_search_tree {

private:
	NODE root;
	int root_centre_x, root_centre_y;
	int node_width;
	float line_color[3] = { 0.0,1.0,0.0 };

	
	NODE get_minimum(NODE temp, NODE *par) {
		if (temp->left == NULL)
			return temp;
		if (temp->right == NULL)
			return temp;
		*par = temp;
		return get_minimum(temp->left, par);
		
	}

public:

	
	binary_search_tree() {
		root = NULL;
		root_centre_x = 700;
		root_centre_y = 600;
		node_width = 25;
	}


	void draw_node(int item, int centre_x, int centre_y, bool write_value = true) {
		char data_string[10];
		glBegin(GL_POLYGON);
		glVertex2f(centre_x + node_width, centre_y + node_width);
		glVertex2f(centre_x - node_width, centre_y + node_width);
		glVertex2f(centre_x - node_width, centre_y - node_width);
		glVertex2f(centre_x + node_width, centre_y - node_width);
		glEnd();
		if (write_value) {
			to_string(data_string, item);
			glColor3f(1.0, 1.0, 0.0);
			display_string(data_string, centre_x - node_width + 10, centre_y-5);
		}
	}

	
	void draw_arrow(int par_x, int par_y, int node_x, int node_y, bool color = true) {
		if (color)
			glColor3f(0.0,1.0,0.0);
		else
			glColor3f(1.0,1.0,1.0);
		glBegin(GL_LINES);
		glVertex2f(par_x, par_y - node_width);
		glVertex2f(node_x, node_y + node_width);
		glEnd();
	}


	NODE insert(int item) {
		draw_tree();
		int num_of_nodes = 1;
		int node_x = root_centre_x, node_y = root_centre_y;
		int par_x, par_y;
		NODE temp, par = NULL, new_node;
		if (root == NULL) {
			root = (NODE)malloc(sizeof(struct node));
			if (root == NULL) {
				printf("There was an error in creating the node\n");
				return NULL;
			}
			root->data = item;
			root->left = root->right = NULL;
			glColor3f(0.0,0.0,0.0);
			draw_node(item, node_x, node_y);
			return root;
		}
		temp = root;
		while (temp != NULL) {
			num_of_nodes *= 2;
			par_x = node_x;
			par_y = node_y;
			node_y -= 100;
			par = temp;
			if (item < temp->data) {
				temp = temp->left;
				node_x -= root_centre_x / num_of_nodes;
			}
			else {
				temp = temp->right;
				node_x += root_centre_x / num_of_nodes;
			}
		}
		new_node = (NODE)malloc(sizeof(struct node));
		if (new_node == NULL) {
			printf("There was an error in creating the node\n");
			return NULL;
		}
		new_node->data = item;
		new_node->left = new_node->right = NULL;
		if (item < par->data)
			par->left = new_node;
		else
			par->right = new_node;
		glColor3f(0.0,0.0,0.0);
		draw_node(item, node_x, node_y);
		draw_arrow(par_x, par_y, node_x, node_y);
		glFlush();
		return root;
	}

	NODE search(int item) {
		draw_tree();
		int num_of_nodes = 1;
		int node_x = root_centre_x, node_y = root_centre_y;
		int par_x, par_y;
		NODE temp, par = NULL, new_node;
		if (root == NULL) {
			glColor3f(1.0, 0.0, 0.0);
			display_string(not_found, 100, 600);
			return root;
		}
		temp = root;
		while (temp != NULL) {
			num_of_nodes *= 2;
			par_x = node_x;
			par_y = node_y;
			par = temp;
			if (item == temp->data) {
				prev_search = true;
				temp_x = node_x;
				temp_y = node_y;
				temp_item = item;
				glColor3f(1.0, 0.0, 0.0);
				draw_node(item, node_x, node_y);
				glFlush();
				return temp;
			}
			else if (item < temp->data) {
				temp = temp->left;
				node_y -= 100;
				node_x -= root_centre_x / num_of_nodes;
			}
			else {
				temp = temp->right;
				node_y -= 100;
				node_x += root_centre_x / num_of_nodes;
			}
		}
		glColor3f(1.0, 0.0, 0.0);
		display_string(not_found, 100, 600);
		glFlush();
		return temp;
	}

	void pre_order(NODE temp, binary_search_tree temp_tree) {
		if (temp != NULL) {
			temp_tree.insert(temp->data);
			pre_order(temp->left, temp_tree);
			pre_order(temp->right, temp_tree);
		}
	}

	void draw_tree() {
		binary_search_tree temp_tree;
		display();
		pre_order(root, temp_tree);
	}

	
	NODE remove(int item) {
		bool found = false;
		int num_of_nodes = 1;
		int node_x = root_centre_x, node_y = root_centre_y;
		int par_x, par_y;
		NODE temp = root, par = root, new_node;
		if (root == NULL) {
			glColor3f(1.0, 0.0, 0.0);
			display_string(not_found, 100, 600);
			return root;
		}
		while (temp != NULL) {
			if (item == temp->data) {
				found = true;
				if (temp == root) {
					if (temp->left == NULL && temp->right == NULL) {
						root = NULL;
						free(temp);
					}
					if (temp->left != NULL || temp->right != NULL) {
						par = temp;
						NODE min_node = get_minimum(temp->right, &par);
						temp->data = min_node->data;
						temp = min_node;
						item = min_node->data;
						continue;
					}
					if (temp->left != NULL ) {
						root = temp->left;
						free(temp);
					}
					if (temp->right != NULL) {
						root = temp->right;
						free(temp);
					}
				}
				else if (temp->left == NULL && temp->right == NULL) {
					if (par->left == temp)
						par->left = NULL;
					else
						par->right = NULL;
					free(temp);
				}
				else if (temp->left != NULL && temp->right != NULL) {
					par = temp;
					NODE min_node = get_minimum(temp->right, &par);
					temp->data = min_node->data;
					temp = min_node;
					item = min_node->data;
					continue;
				}

				else {
					if (temp->left != NULL )
						(par->left == temp) ? (par->left = temp->left) : (par->right = temp->left);
					else
						(par->left == temp) ? (par->left = temp->right) : (par->right = temp->right);
					free(temp);
				}
				break;
			}
			par = temp;
			if (item < temp->data)
				temp = temp->left;
			else
				temp = temp->right;
		}
		if (!found) {
			glColor3f(1.0, 0.0, 0.0);
			display_string(not_found, 100, 600);
		}
		else
			draw_tree();
	}
	void accept_tree() {
		int n;
		printf_s("\nCREATING BST: (enter 0 to finish)\n");
		while (1) {
			fflush(stdin);
			printf_s("\nEnter node value: ");
			scanf_s("%d", &n);
			if (n == 0) {
				draw_tree();
				break;
			}
			insert(n);
			draw_tree();
			
		}
	}
	


};

 
binary_search_tree tree;
NODE(binary_search_tree::*operation)(int) = NULL;

class option_box {

private:
	float bottom_corner_x, bottom_corner_y;
	float bc;
	float height, width;
	float color[3];
	char option_name[20];

public:
	option_box() {
		bottom_corner_x = 0;
		bottom_corner_y = 0;
		width = 0;
		height = 0;
		color[RED] = 0.0;
		color[GREEN] = 0.0;
		color[BLUE] = 1.0;
		strcpy_s(option_name, "");
	}

	
	option_box(const char name[]) {
		bottom_corner_x = 550;
		bottom_corner_y = 700;
		width = 100;
		height = 50;
		color[RED] = 0.0;
		color[GREEN] = 0.0;
		color[BLUE] = 1.0;
		strcpy_s(option_name, name);
	}

	
	void draw_box() {
		
		bottom_corner_x += offset;
		bc = bottom_corner_x;
		glColor3f(color[RED], color[GREEN], color[BLUE]);
		glBegin(GL_QUADS);
		glVertex2f(bottom_corner_x, bottom_corner_y);
		glVertex2f(bottom_corner_x + width, bottom_corner_y);
		glVertex2f(bottom_corner_x + width, bottom_corner_y + height);
		glVertex2f(bottom_corner_x, bottom_corner_y + height);
		glEnd();
		offset += 120.0;
		glColor3f(1.0, 1.0, 0.0);
		display_string(option_name, bottom_corner_x + 10.0, bottom_corner_y + 20.0);
	}

	
	bool clicked(int x, int y) {
		if (x > bc && x < bc + width)
			if (y > bottom_corner_y && y < bottom_corner_y + height) {
				box_clicked = true;
				return true;
			}
		return false;
	}
};

option_box insert_box, search_box, delete_box;


void init() {
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, SCREEN_SIZE_X, 0.0, SCREEN_SIZE_Y);

}



void displayOptions() {
	option_box insert_box("Insert"), search_box("Search"), delete_box("Delete");
	offset = 0.0;
	insert_box.draw_box();
	search_box.draw_box();
	delete_box.draw_box();
	
	::insert_box = insert_box;
	::search_box = search_box;
	::delete_box = delete_box;
}



void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	displayOptions();
	glFlush();
}

void mouse(int button, int state, int x, int y) {
	y = SCREEN_SIZE_Y - y;
	tree.draw_tree();
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			
		if (prev_search == true) {
			glColor3f(0.0,0.0,0.0);
			tree.draw_node(temp_item, temp_x, temp_y);
			tree.draw_tree();
			prev_search = false;
		}
		if (insert_box.clicked(x, y)) {
			operation = &binary_search_tree::insert;
			clear_input_region();
			glColor3f(1.0, 0.0, 0.0);
			display_string(query_string, 100, 600);
			glFlush();
		}
		if (search_box.clicked(x, y)) {
			operation = &binary_search_tree::search;
			clear_input_region();
			glColor3f(1.0, 0.0, 0.0);
			display_string(query_string, 100, 600);
			glFlush();
		}
		if (delete_box.clicked(x, y)) {
			operation = &binary_search_tree::remove;
			clear_input_region();
			glColor3f(1.0, 0.0, 0.0);
			display_string(query_string, 100, 600);
			glFlush();
		}
		tree.draw_tree();
	}

}



void keyboard(unsigned char key, int x, int y) {
	char s[2];
	static int digit = 0;
	static int number = 0;
	static int sign = 1;
	
	if (!box_clicked) {
		tree.draw_tree();
		return;
	}
		
	if (digit == 0 && key == '-') {
		s[0] = key;
		s[1] = '\0';
		glColor3f(1.0, 0.0, 0.0);
		display_string(s, 420 + (15 * digit), 600);
		glFlush();
		sign = -1;
	}
	else if (key != 13 && isdigit(key)) {
		digit++;
		number = (number * 10) + key - '0';
		s[0] = key;
		s[1] = '\0';
		glColor3f(1.0, 0.0, 0.0);
		display_string(s, 420 + (15 * digit), 600);
		glFlush();
	}
	else if (key != 13 && !isdigit(key));
	else {
		digit = 0;
		clear_input_region();
		(tree.*operation)(sign*number);
		number = 0;
		sign = 1;
		box_clicked = false;
	}
}



void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	glutInitWindowPosition(0,0);
	tree.accept_tree();
	glutCreateWindow("Binary Search Tree");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	init();
	glutMainLoop();
	
}


