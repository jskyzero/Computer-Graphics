#ifndef __OPENGL_HELPER__
#define __OPENGL_HELPER__

/*
 * ┌───────────────────────────────────────────────────────────────────────────┐
 * |                                                                           |
 * |                               OpenGL Helper                               |
 * ╠═══════════════════════════════════════════════════════════════════════════╣
 * |                                                            jskyzero 2018  |
 * └───────────────────────────────────────────────────────────────────────────┘
 *
 *
 * 1.Introdution
 * OpenGL Helper is a help header file library, It contails many useful
 * functions when you use OpenGl.
 *
 * 2.Requirment
 * Require Library:
 *  GLFW
 *  GLAD
 *  GLM
 *  stb_image
 *  imgui(optional) -> not use please #define NOT_USE_IMGUI
 *
 */


// for glad, initial function pointer address(maybe
#include <glad/glad.h>
// for glfw, a OpenGL, OpenGL ES and Vulkan library
#include <GLFW/glfw3.h>
// for glm, OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


// for stb_image, a image loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef NOT_USE_IMGUI
  // for imgui, a c++ ui library
  #include "imgui.h"
  #include "imgui_impl_glfw_gl3.h"
#endif

#include <cstdlib>     // for exit
#include <iostream>    // for std::cout
#include <fstream>     // for std::ifstream
#include <sstream>     // for std::stringstream
#include <functional>  // for function
#include <vector>

#define INFO_LOG_SIZE 512

// namespace helper
namespace helper {

/*
 *
 * COMMON FUNCTIONS
 *
 *
 *
 * */

// return some info
std::string version() { return "OpenGL Helper 0.0.1 by jskyzero"; }

// exit with exit code check
void exit_program(int exit_code) {
  glfwTerminate();
  exit(0);
}
// commen exit
void exit_program() { exit_program(0); }

// exit with error message
void exit_program(std::string exit_message) {
  std::cout << exit_message << std::endl;
  exit_program(-1);
}

// test if error
void assert_true(bool isOK, std::string error_message) {
  if (isOK) return;
  exit_program(error_message);
}

// read string from path
std::string read_string_from_path(std::string path) {
  std::ifstream in;
  std::stringstream ss;
  // ensure ifstream objects can throw exceptions:
  in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    in.open(path.c_str());
    // read file's buffer contents into streams
    ss << in.rdbuf();
    // close file handlers
    in.close();
  } catch (std::ifstream::failure e) {
    std::string error_message = "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
    error_message += "\nFile path: ";
    error_message += path;
    exit_program(error_message);
  }
  // convert stream into string
  return ss.str();
}

/*
 *
 * OPENGL FUNCTIONS
 *
 *
 *
 * */

// initial opengl
void initial_glfw() {
  assert_true(glfwInit() == GLFW_TRUE, "glfw initial error");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #ifdef __APPLE__
    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE);  // uncomment this statement to fix compilation on OS X
  #endif
}

// initial glad
void initial_glad() {
  assert_true(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0,
              "glad load errror");
}

#ifndef NOT_USE_IMGUI

// initial imgui
void initial_imgui(GLFWwindow* window) {
  // Setup ImGui binding
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
  // Controls  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable
  // Gamepad Controls
  ImGui_ImplGlfwGL3_Init(window, true);

  // Setup style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default_CAMERA font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored
  // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
  // ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'misc/fonts/README.txt' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese());  IM_ASSERT(font != NULL);
}

#endif

// inital opengl
void InitialOpenGL(std::function<void(void)> set_window, GLFWwindow*& window) {
  initial_glfw();
  set_window();
  initial_glad();
  #ifndef NOT_USE_IMGUI
    initial_imgui(window);
  #endif
  glViewport(0, 0, 800, 600);
}

// set vao
void SetVAO(GLuint& VAO, GLuint& VBO, GLuint& EBO,
             std::function<void(GLuint, GLuint, GLuint)> set_buffer,
             bool is_buffer_need_initial = true) {
  glGenVertexArrays(1, &VAO);
  if (is_buffer_need_initial) glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  set_buffer(VAO, VBO, EBO);
}

typedef PFNGLGETSHADERIVPROC CheckShaderHasErrorFunc;
typedef PFNGLGETSHADERINFOLOGPROC GetShaderErrorMessageFunc;

// check if shader works well
void CheckShaderHaveError(GLuint id, GLenum type,
                             CheckShaderHasErrorFunc check_has_error,
                             GetShaderErrorMessageFunc get_error_info) {
  GLint no_error;
  GLchar info_log[INFO_LOG_SIZE];
  check_has_error(id, type, &no_error);
  if (no_error) return;
  get_error_info(id, INFO_LOG_SIZE, NULL, info_log);
  exit_program(info_log);
}

// compile shader
GLuint CompileShader(GLenum type, std::string path) {
  GLuint shader = glCreateShader(type);
  std::string str = read_string_from_path(path);
  const GLchar* source = str.c_str();
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  CheckShaderHaveError(shader, GL_COMPILE_STATUS, glGetShaderiv,
                          glGetShaderInfoLog);
  return shader;
}

// compile vertex shader
GLuint CompileVertexShader(std::string path) {
  return CompileShader(GL_VERTEX_SHADER, path);
}

// compile fragment shader
GLuint CompileFragmentShader(std::string path) {
  return CompileShader(GL_FRAGMENT_SHADER, path);
}

// create program and link shader
GLuint CreatProgramWithShader(GLuint vertex_shader,
                                  GLuint fragment_shader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  CheckShaderHaveError(program, GL_LINK_STATUS, glGetProgramiv,
                          glGetProgramInfoLog);
  return program;
}

// create program and link shader with file path
GLuint CreatProgramWithShader(std::string vertex_shader_path,
                                  std::string fragment_shader_path) {
  GLuint vertex_shader = CompileVertexShader(vertex_shader_path);
  GLuint fragment_shader = CompileFragmentShader(fragment_shader_path);
  GLuint program = CreatProgramWithShader(vertex_shader, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return program;
}

// create texture with file path
void CreateTexture(GLuint& texture, std::string file_path) {
  // load and create a texture
  // -------------------------
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);  // all upcoming GL_TEXTURE_2D
                                          // operations now have effect on this
                                          // texture object
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT);  // set texture wrapping to GL_REPEAT (default
                               // wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(false);
  // std::string str = read_string_from_path(file_path);
  unsigned char* data =
      stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
  assert_true(data, "Failed to load texture " + file_path);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}

// set int
void SetShaderInt(GLuint id, const std::string& name, int value) {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
// set float
void SetShaderFloat(GLuint id, const std::string& name, float value) {
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
// set vec3
void SetShaderVec3(GLuint id, const std::string& name, const glm::vec3& value){
  glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
// set vec3
void SetShaderVec3(GLuint id, const std::string& name, float x, float y, float z){
  glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
// set mat4
void SetShaderMat4(GLuint id, const std::string& name, const glm::mat4& mat) {
  // std::cout << glm::to_string(mat) << std::endl;
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}



// Default camera values
const float kDefaultCameraYaw         = -90.0f;
const float kDefaultCameraPitch       =  0.0f;
const float kDefaultCameraSpeed       =  2.0f;
const float kDefaultCameraSensivity =  0.1f;
const float kDefaultCameraZoom        =  45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
 public:
  // Defines several possible options for camera movement. Used as abstraction
  // to stay away from window-system specific input methods
  enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };
  // Camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Euler Angles
  float Yaw;
  float Pitch;
  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = kDefaultCameraYaw, float pitch = kDefaultCameraPitch)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(kDefaultCameraSpeed),
        MouseSensitivity(kDefaultCameraSensivity),
        Zoom(kDefaultCameraZoom) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
  }
  // Constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(kDefaultCameraSpeed),
        MouseSensitivity(kDefaultCameraSensivity),
        Zoom(kDefaultCameraZoom) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
  }

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
  }

  // Processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) Position += Front * velocity;
    if (direction == BACKWARD) Position -= Front * velocity;
    if (direction == LEFT) Position -= Right * velocity;
    if (direction == RIGHT) Position += Right * velocity;
  }

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (Pitch > 89.0f) Pitch = 89.0f;
      if (Pitch < -89.0f) Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
  }

  // Processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset) {
    if (Zoom >= 1.0f && Zoom <= 45.0f) Zoom -= yoffset;
    if (Zoom <= 1.0f) Zoom = 1.0f;
    if (Zoom >= 45.0f) Zoom = 45.0f;
  }

 private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void UpdateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(
        Front, WorldUp));  // Normalize the vectors, because their length gets
                           // closer to 0 the more you look up or down which
                           // results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
  }
};

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh {
 public:
  /*  Mesh Data  */
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int VAO;

  /*  Functions  */
  // constructor
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // now that we have all the required data, set the vertex buffers and its
    // attribute pointers.
    setupMesh();
  }

  // render the mesh
  void Draw(GLuint shader) {
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 +
                      i);  // active proper texture unit before binding
      // retrieve texture number (the N in diffuse_textureN)
      std::string number;
      std::string name = textures[i].type;
      if (name == "texture_diffuse")
        number = std::to_string(diffuseNr++);
      else if (name == "texture_specular")
        number =
            std::to_string(specularNr++);  // transfer unsigned int to stream
      else if (name == "texture_normal")
        number = std::to_string(normalNr++);  // transfer unsigned int to stream
      else if (name == "texture_height")
        number = std::to_string(heightNr++);  // transfer unsigned int to stream

      // now set the sampler to the correct texture unit
      glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
      // and finally bind the texture
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
  }

 private:
  /*  Render data  */
  unsigned int VBO, EBO;

  /*  Functions    */
  // initializes all the buffer objects/arrays
  void setupMesh() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for
    // all its items. The effect is that we can simply pass a pointer to the
    // struct and it translates perfectly to a glm::vec3/2 array which again
    // translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
  }
};




unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


class Model 
{
public:
    /*  Model Data */
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    Model(std::string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(GLuint shader)
    {
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    
private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
          // std::cout << i << " ";
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // Walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};


}  // namespace helper

#endif