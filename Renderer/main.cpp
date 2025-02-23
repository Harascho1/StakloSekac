//Renderer main

#include <time.h>
#include "include/VertexArray.h"
#include "include/Renderer.h"
#include "include/VertexBuffer.h"
#include "include/IndexBuffer.h"
#include "include/Texture.h"
#include "Shaders/include/Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <windows.h>

struct Character {
    unsigned int Texture_ID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

struct StackLinija {
    float *lines_array = nullptr;
    unsigned int *lines_indencies = nullptr;
    float *number_indencies = nullptr;
    const float razmak = 5.0f;

    int size = 0;
    int pos_size = 0;
    void push(int posX, int posY, int width, int height) {

        int newSize = size + 8;
        int new_pos_size = newSize / 4;
        float *tmp = (float*)realloc(lines_array, newSize * sizeof(float));
        unsigned int *tmp_indencies = (unsigned int*)realloc(lines_indencies, newSize * sizeof(unsigned int));
        float *tmp_numbers = (float*)realloc(number_indencies, new_pos_size  * sizeof(float));


        if (size == 0) {
            tmp_indencies[0] = 0;
            tmp_indencies[1] = 1;
            tmp_indencies[2] = 1;
            tmp_indencies[3] = 2;
            tmp_indencies[4] = 2;
            tmp_indencies[5] = 3;
            tmp_indencies[6] = 3;
            tmp_indencies[7] = 0;
        }
        else {
            for (int i = 0; i < size; i++) {
                tmp[i] = lines_array[i];
                tmp_indencies[i] = lines_indencies[i];
            }
            for (int i = 0; i < pos_size; i++) {
                tmp_numbers[i] = number_indencies[i];
            }
            int tmp_size = size;
            while (tmp_size < newSize) {
                tmp_indencies[tmp_size] = lines_indencies[tmp_size-8] + 4;
                tmp_size++;
            }
        }
        tmp[size++] = posX * 1.0f;          tmp[size++] = posY * 1.0f;
        tmp[size++] = width * 1.0f + posX;  tmp[size++] = posY * 1.0f;
        tmp[size++] = width * 1.0f + posX;  tmp[size++] = posY + height * 1.0f;
        tmp[size++] = posX * 1.0f;          tmp[size++] = posY + height * 1.0f;

        tmp_numbers[pos_size++] = posX * 1.0f + razmak;  
        tmp_numbers[pos_size++] = posY * 1.0f + razmak;  
        
        lines_array = tmp;
        lines_indencies = tmp_indencies;
        number_indencies = tmp_numbers;
        size = newSize;
        pos_size = new_pos_size;

    }
    void print() const {
        // for (int i = 0; i < size; i++) {
        //     printf("%u ", lines_indencies[i]);
        // }
        // printf("\n");
        // for (int i = 0; i < size; i++) {
        //     printf("%f ", lines_array[i]);
        // }
        // printf("\n");
        for (int i = 0; i < pos_size; i++) {
            printf("%f ", number_indencies[i]);
        }
        printf("\n");
    }
    ~StackLinija() {
        free(lines_array);
        free(lines_indencies);
        free(number_indencies);
    }
};

float plateWidth, plateHeight;
std::map<char, Character> Characters;

// Declarations
void framebuffer_size_callback(GLFWwindow *widnow, int width, int height);
int initCharacters(FT_Face& face);
void RenderText(const VertexArray& vArray, const VertexBuffer& vBuffer, Shader& shader, int size, float* buffer, float scale);
void processInputs(GLFWwindow * window);

// argv[1] = pozicija_X, argv[2] = pozicija_Y, argv[3] = duzina, argv[4] = sirina
// argv[1] = 0.0, argv[2] = 0.0, argv[3] = 250, argv[4] = 500
int main(int argc, char *argv[]) {
    // std::cout << argc << '\n';

    if (argc == 3) {
        // std::cout << argv[1] << ' ' << argv[2] << '\n';
        plateWidth = atoi(argv[1]) * 1.0f;
        plateHeight = atoi(argv[2]) * 1.0f;
    }
    HANDLE hPipe;
    ConnectPipe(&hPipe);

    char buffer[1024];
    DWORD bytesRead, bytesWritten;
    StackLinija stack;
    while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        buffer[bytesRead] = '\0';
        WriteFile(hPipe, "RECEIVED", strlen("RECEIVED\0"), &bytesWritten, NULL);
        int posX, posY, width, height;
        sscanf(buffer, "%d %d %d %d", &posX, &posY, &width, &height);
        // printf("%d %d %d %d", posX, posY, width, height);
        stack.push(posX, posY, width, height);
    }
    CloseHandle(hPipe);
    //Inicijalizacija
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int width = 1400, height = 1000;
    float ratio = width / height * 1.0f;
    GLFWwindow* window = glfwCreateWindow(width, height, "Staklo Sekac", nullptr, nullptr);
    if (!window) {
        std::cout << "FAILED TO INITIALIZE WINDOW\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "FAILED TO INITIALIZE GLAD\n";
        return -1;
    }
    glViewport(0, 0, width, height);

    //inicijalizijacija fonta
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "FAILED TO INITIALIZE FREETYPE LIB\n";
        return -1;
    }
    FT_Face face;
    if (FT_New_Face(ft, "../dependencies/fonts/CONSOLA.ttf", 0, &face)) {
        std::cout << "FAILED TO LOAD A FONT\n";
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 20);
    initCharacters(face);
    
    float plate_array[8] {
        0.0f, 0.0f,    
        plateWidth, 0.0f,
        0.0f, plateHeight,
        plateWidth, plateHeight
    };

    unsigned int plate_indecies[6] {
        0, 1, 2,	//1. trougao
        1, 2, 3,	//2. trougao
    };
    {

    glm::mat4 proj = glm::ortho(-30.0f, 930.0f, 930.0f, -30.0f);

    //* OVO MORA DA BI SE SLIKE PNG FORMATA LEPO PRIKAZELE!!!
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Renderer renderer;

    // definise se koji GLSR ce shader da koristi
    Shader shaderBoja("../Renderer/Shaders/Transform.shader");
    shaderBoja.Bind();
    shaderBoja.setUniform4f("u_Color", 0.1, 0.3, 0.4, 0.5);
    shaderBoja.setUniformMatrix4fv("u_Transform", proj);

    Shader shaderText("../Renderer/Shaders/Text.shader");
    shaderText.Bind();
    shaderText.setUniformMatrix4fv("u_Projection", proj);
    shaderText.setUniform3f("u_textColor", 1.0f, 1.0f, 1.0f);


    //rectangle
    VertexArray va;
    VertexBuffer vb(plate_array, sizeof(plate_array));
    VertexBufferLayout* layout = new VertexBufferLayout();
    layout->Push(GL_FLOAT, 2);
    va.AddBuffer(vb, *layout);
    IndexBuffer ib(plate_indecies, 6);
    delete layout;

    //lines
    VertexArray linije_vertex;
    VertexBuffer linije_buffer(stack.lines_array, stack.size * sizeof(float));
    VertexBufferLayout *linije_layout = new VertexBufferLayout();
    linije_layout->Push(GL_FLOAT, 2);
    linije_vertex.AddBuffer(linije_buffer, *linije_layout);
    IndexBuffer linije_index(stack.lines_indencies, stack.size);
    delete linije_layout;
    
    float texCoord[] {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    VertexArray text_vertex;
    VertexBuffer text_buffer(texCoord, sizeof(texCoord));
    VertexBufferLayout *text_layout = new VertexBufferLayout();
    text_layout->Push(GL_FLOAT, 2);
    text_vertex.AddBuffer(text_buffer, *text_layout);
    delete text_layout;

    //render loop
    while (!glfwWindowShouldClose(window)) {
        renderer.clear();
        renderer.Draw(va, ib, shaderBoja);
        RenderText(text_vertex, text_buffer, shaderText, stack.pos_size, stack.number_indencies, 1.0f);
        renderer.DrawLines(linije_vertex, linije_index, shaderBoja);
        processInputs(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int initCharacters(FT_Face& face) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned int c = 0; c < 65; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "FAILED TO LOAD GYPTH\n";
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        Characters.insert(std::pair<char, Character> (c, character));
    }
    return 0;

}

void RenderText(const VertexArray& vArray, const VertexBuffer& vBuffer, Shader& shader, int size, float* buffer, float scale) {

    shader.Bind();
    glActiveTexture(GL_TEXTURE0);
    unsigned int offset = 0;
    vArray.Bind();

    std::string::const_iterator c;
    for (int i = 0; i < size; i+=2) {
        int broj = i / 2 + 1;
        std::string text = std::to_string(broj);
        float tmp = buffer[i];
        for (c = text.begin(); c != text.end(); c++) {
            Character ch = Characters[*c];

            float xpos = tmp + ch.Bearing.x * scale;
            float ypos = buffer[i+1] + (ch.Size.y - ch.Bearing.y) * scale;

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(ch.Size.x * scale, ch.Size.y * scale, 0));
            shader.setUniformMatrix4fv("u_Transform", transform);

            glBindTexture(GL_TEXTURE_2D, ch.Texture_ID);
            vBuffer.Bind();
            // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            // vBuffer.Unbind();
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0 , 4, 1);

            // * ovaj printf mi pokazuje da mi ofset dobro radi
            // printf("offset mi je -> %d za slovo %c\n", (int)offset, *c);

            tmp += (ch.Advance >> 6) * scale;
            // offset += sizeof(vertices);
        }
    }

    vArray.Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);

}

void processInputs(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
