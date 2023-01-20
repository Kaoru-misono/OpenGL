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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	const unsigned int screen_width = 1600, screen_height = 1200;
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "OPenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
	

	std::shared_ptr<Vertex_Array> rectangle_vertex_array = std::make_shared<Vertex_Array>();

	std::shared_ptr<Vertex_Buffer> vertex_buffer_first = std::make_shared<Vertex_Buffer>(box_vertices, sizeof(box_vertices));
	vertex_buffer_first->set_layout(layout2);

	std::shared_ptr< Index_Buffer> index_buffer = std::make_shared<Index_Buffer>(indices, sizeof(indices)/sizeof(uint32_t));

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



	//MVP
	//model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	//view
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	//projection
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);



	//Shader
	Shader triangle_shader("Asset/Shader/triangle-vert.glsl", "Asset/Shader/triangle-frag.glsl");


	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);


	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	triangle_shader.bind();
	triangle_shader.set_int("texture1", 0);
	triangle_shader.set_int("texture2", 1);

	

	//render loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.8f, 0.5f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		



		rectangle_vertex_array->bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cube_position[i]);
			float angle = 20 * (i + 1);
			model = glm::rotate(model, sin_factor * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glm::mat4 mvp = projection * view * model;
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

		//glfw: swap buffersand poll IO events(keys pressed / released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	index_buffer->unbind();
	triangle_shader.unbind();

	// glfw: terminate, clearing all previously allocated GLFW resources.
   // ------------------------------------------------------------------
	glfwTerminate();
	return 0;

}