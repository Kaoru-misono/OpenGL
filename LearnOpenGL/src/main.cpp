#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Renderer/shader.h"
#include "Renderer/buffer.h"
#include "Renderer/vertex-array.h"
#include "Renderer/camera.h"

static bool first_mouse = true;
static const unsigned int screen_width = 1600, screen_height = 1200;
static float last_x = (float)screen_width / 2.0f, last_y = (float)screen_height / 2.0f;

Perspective_Camera camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(-20.0f, -80.0, 0.0f));

//delta time
static float delta_time = 0.0f;
static float last_frame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (first_mouse)
	{
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}
	float x_offset = x_pos - last_x;
	float y_offset = last_y - y_pos;
	last_x = x_pos;
	last_y = y_pos;

	camera.process_mouse_event(x_offset, y_offset);

}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	camera.process_scroll_event(static_cast<float>(y_offset));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow* window)
{
	//close
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//camera controller
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.process_key_event(FORWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.process_key_event(BACKWARD, delta_time);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.process_key_event(LEFT, delta_time);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.process_key_event(RIGHT, delta_time);

}

int main()
{
	//Init
	//-----------------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	//------------------------------------------------------------------------

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "OPenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Glad Initialized failed!" << std::endl;
		return -1;
	}
	//------------------------------------------------------------------------------

	//Create an array of vertices
	float vertices[3 * 8] =
	{
		//pos				//Color				//texcoord
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 0.5f, 1.0f
	};
	//rectangle vertices
	float rectangle_vertices[] =
	{
		//pos				//Color				//texcoord
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f, 1.0f
	};

	//array of vertices has Two Triangles 
	float vertices_of_two_triangles[6 * 3] =
	{
		-0.9f, -0.5f, 0.0f,
		 0.0f, -0.5f, 0.0f,
		-0.45f, 0.5f, 0.0f,
		-0.0f, -0.5f, 0.0f,
		 0.9f, -0.5f, 0.0f,
		 0.45f, 0.5f, 0.0f
	};
	//Box vertices
	float box_vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//cube position
	glm::vec3 cube_position[] =
	{
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//Array of indices
	unsigned int indices[] = 
	{ 
		0, 1, 2,
		2, 3, 1
	};

	Buffer_Layout layout = {
		{Shader_Data_Type::Float3, "a_Position"},
		{Shader_Data_Type::Float3, "a_Color"},
		{Shader_Data_Type::Float2, "a_texcoord"}
	};
	Buffer_Layout layout2 = {
		{Shader_Data_Type::Float3, "a_Position"},
		{Shader_Data_Type::Float2, "a_texcoord"}
	};

	//What do we need to draw an object on screen?
	//Vertex Array, Vertex Buffer, Index Buffer and shader
	

#if CHAPTER_ONE
	std::shared_ptr<Vertex_Array> rectangle_vertex_array = std::make_shared<Vertex_Array>();

	std::shared_ptr<Vertex_Buffer> vertex_buffer_first = std::make_shared<Vertex_Buffer>(box_vertices, sizeof(box_vertices));
	vertex_buffer_first->set_layout(layout2);

	std::shared_ptr< Index_Buffer> index_buffer = std::make_shared<Index_Buffer>(indices, sizeof(indices) / sizeof(uint32_t));

	rectangle_vertex_array->add_vertex_buffer(vertex_buffer_first);
	//rectangle_vertex_array->set_index_buffer(index_buffer);

	//-----------------------texture-------------------------------------
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//GL_REPEAT GL_MIRRORED_REPEAT GL_CLAMP_TO_EDGE GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//GL_LINEAR GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load image
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load("Asset/texture/leidian.jpg", &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load image!" << std::endl;
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load image2
	data = stbi_load("Asset/texture/container.jpg", &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load image!" << std::endl;
	stbi_image_free(data);

	//Shader
	Shader triangle_shader("Asset/Shader/triangle-vert.glsl", "Asset/Shader/triangle-frag.glsl");
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	triangle_shader.bind();
	triangle_shader.set_int("texture1", 0);
	triangle_shader.set_int("texture2", 1);
#endif // CHAPTER_ONE


	std::shared_ptr<Vertex_Array> box_VAO = std::make_shared<Vertex_Array>();
	std::shared_ptr<Vertex_Buffer> box_VBO = std::make_shared<Vertex_Buffer>(box_vertices, sizeof(box_vertices));
	box_VBO->set_layout(layout2);

	box_VAO->add_vertex_buffer(box_VBO);

	std::shared_ptr<Vertex_Array> light_VAO = std::make_shared<Vertex_Array>();
	light_VAO->add_vertex_buffer(box_VBO);

	

	//create view matrix( = lookat() )
	/*glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 camera_right = glm::cross(up, camera_direction);
	glm::vec3 camera_up = glm::cross(camera_direction, up);*/

	



	//MVP
	//model
	glm::mat4 model = glm::mat4(1.0f);
	//view
	glm::mat4 view = glm::mat4(1.0f);
	//projection
	glm::mat4 projection = glm::mat4(1.0f);

	//light
	glm::vec3 light_pos(1.2f, 1.0f, 2.0f);
	
	//camera.set_euler(glm::vec3(10.0f, -90.0f, 0.0f));



	

	Shader box_shader("Asset/Shader/box-vert.glsl", "Asset/Shader/box-frag.glsl");
	Shader light_shader("Asset/Shader/light-vert.glsl", "Asset/Shader/light-frag.glsl");


	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	

	

	//render loop
	while(!glfwWindowShouldClose(window))
	{
		//update delta time
		float current_frame = (float)glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		process_input(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 mvp = glm::mat4(1.0f);
		
		

		model = glm::mat4(1.0f);
		mvp = camera.get_view_projection_matrix() * model;

		box_shader.bind();
		box_shader.set_float4("light_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		box_shader.set_float4("object_color", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
		box_shader.set_mat4("u_mvp", mvp);
		box_VAO->bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		light_VAO->bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.2f));
		mvp = camera.get_view_projection_matrix() * model;
		light_shader.bind();
		light_shader.set_mat4("u_mvp", mvp);
		glDrawArrays(GL_TRIANGLES, 0, 36);





#if CHAPTER_ONE
		//ShaderProgram Use
		triangle_shader.bind();

		//calculate uniform
		float time_value = (float)glfwGetTime();
		float sin_factor = (sin(time_value) / 2.f) + 0.5f;
		glm::vec4 color(0.0f, sin_factor, 0.0f, 1.0f);
		/*
		//translate
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, time_value, glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		*/
		//model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.5f, 1.0f, 0.0f));

		//Set Uniform

		triangle_shader.set_float4("u_Color", color);
		triangle_shader.set_float("u_time_factor", sin_factor);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		float radius = 10.0f;
		//camera_pos.x = cos(time_value) * radius; //x = r.sin(theta)
		//camera_pos.z = sin(time_value) * radius; //z = r.cos(theta)
		// camera_front = glm::vec3(0.0f, 0.0f, 0.0f) - camera_pos;



		//std::cout << camera.get_euler().x << " " << camera.get_euler().y << std::endl;



		rectangle_vertex_array->bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cube_position[i]);
			float angle = 20 * (i + 1);
			model = glm::rotate(model, sin_factor * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glm::mat4 mvp = camera.get_view_projection_matrix() * model;
			triangle_shader.set_mat4("u_mvp", mvp);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/*
		//Draw second times
		trans = glm::mat4(1.0f);
		trans = glm::scale(trans, glm::vec3(sin_factor));
		triangle_shader.set_mat4("u_transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		*/

#endif
		//glfw: swap buffersand poll IO events(keys pressed / released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	

	// glfw: terminate, clearing all previously allocated GLFW resources.
   // ------------------------------------------------------------------
	glfwTerminate();
	return 0;

}