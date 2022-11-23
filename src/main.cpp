// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <string>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// Function Prototypes
bool LoadTextureFromFile(const char*, GLuint*, int*, int*);
int  arrangeDigits(int, int);

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static float fontSize = 36.0;


int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);

     

    
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(2000, 2000, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* def = io.Fonts->AddFontDefault();
    ImFont* ubuntu;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();
    int digit = 0;
    GLuint image_button[15];
    int my_image_width = 0;
    int my_image_height = 0;
    bool ret[15];
    ret[0] = LoadTextureFromFile("../images/button_0.png", &image_button[0], &my_image_width, &my_image_height);
    ret[1] = LoadTextureFromFile("../images/button_1.png", &image_button[1], &my_image_width, &my_image_height);
    ret[2] = LoadTextureFromFile("../images/button_2.png", &image_button[2], &my_image_width, &my_image_height);
    ret[3] = LoadTextureFromFile("../images/button_3.png", &image_button[3], &my_image_width, &my_image_height);
    ret[4] = LoadTextureFromFile("../images/button_4.png", &image_button[4], &my_image_width, &my_image_height);
    ret[5] = LoadTextureFromFile("../images/button_5.png", &image_button[5], &my_image_width, &my_image_height);
    ret[6] = LoadTextureFromFile("../images/button_6.png", &image_button[6], &my_image_width, &my_image_height);
    ret[7] = LoadTextureFromFile("../images/button_7.png", &image_button[7], &my_image_width, &my_image_height);
    ret[8] = LoadTextureFromFile("../images/button_8.png", &image_button[8], &my_image_width, &my_image_height);
    ret[9] = LoadTextureFromFile("../images/button_9.png", &image_button[9], &my_image_width, &my_image_height);
    ret[10] = LoadTextureFromFile("../images/button_multiply.png", &image_button[10], &my_image_width, &my_image_height);
    ret[11] = LoadTextureFromFile("../images/button_minus.png", &image_button[11], &my_image_width, &my_image_height);
    ret[12] = LoadTextureFromFile("../images/button_divide.png", &image_button[12], &my_image_width, &my_image_height);
    ret[13] = LoadTextureFromFile("../images/button_plus.png", &image_button[13], &my_image_width, &my_image_height);
    ret[14] = LoadTextureFromFile("../images/button_equals.png", &image_button[14], &my_image_width, &my_image_height);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ubuntu = io.Fonts->AddFontFromFileTTF("../fonts/ubuntu/UbuntuMono-R.ttf", fontSize);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::PushFont(ubuntu);

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
            

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            
            ImGui::Begin("Hello, world!");
            // Create a window called "Hello, world!" and append into it.
            
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::Text("Note, this does not set the background color anymore because I set it manually.\nHowever, I think this is cool and I may use it for something later on.");

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            int i = 0;

            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            
            glfwInit();

            
            
            
            
            
            //display results
            ImGui::Text("%d", digit);
            

            
            if (ImGui::ImageButton("button1", (void*)(intptr_t)image_button[1], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 1);
            }
            ImGui::SameLine(216, 0);
            if (ImGui::ImageButton("button2", (void*)(intptr_t)image_button[2], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 2);
            }
            ImGui::SameLine(424, 0);
            if (ImGui::ImageButton("button3", (void*)(intptr_t)image_button[3], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 3);
            }
            ImGui::SameLine(632, 0);
            if (ImGui::ImageButton("multiply_button", (void*)(intptr_t)image_button[10], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                
            }
            if (ImGui::ImageButton("button4", (void*)(intptr_t)image_button[4], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 4);
            }
            ImGui::SameLine(216, 0);
            if (ImGui::ImageButton("button5", (void*)(intptr_t)image_button[5], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 5);
            }
            ImGui::SameLine(424, 0);
            if (ImGui::ImageButton("button6", (void*)(intptr_t)image_button[6], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 6);
            }
            ImGui::SameLine(632, 0);
            if (ImGui::ImageButton("minus_button", (void*)(intptr_t)image_button[11], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                
            }
            if (ImGui::ImageButton("button7", (void*)(intptr_t)image_button[7], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 7);
            }
            ImGui::SameLine(216, 0);
            if (ImGui::ImageButton("button8", (void*)(intptr_t)image_button[8], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 8);
            }
            ImGui::SameLine(424, 0);
            if (ImGui::ImageButton("button9", (void*)(intptr_t)image_button[9], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 9);
            }
            ImGui::SameLine(632, 0);
            if (ImGui::ImageButton("plus_button", (void*)(intptr_t)image_button[13], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                
            }
            if (ImGui::ImageButton("divide_button", (void*)(intptr_t)image_button[12], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                
            }
            ImGui::SameLine(216, 0);
            if (ImGui::ImageButton("button0", (void*)(intptr_t)image_button[0], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = arrangeDigits(digit, 0);
            }
            ImGui::SameLine(632, 0);
            if (ImGui::ImageButton("equals_button", (void*)(intptr_t)image_button[14], ImVec2((float)200, (float)200), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 0.0f, 0.0f, 1.0f)))
            {
                digit = 0;
            }
            ImGui::NewLine();
            ImGui::NewLine();
            ImGui::Text("%d", digit);
            ImGui::NewLine();
            ImGui::NewLine();

            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

       

        // Rendering
        ImGui::PopFont();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0,.66,1.0,1.0); //Set the background color (R, G, B, A) float Scale 0 - 1
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        glfwSwapBuffers(window);
        
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// Arrange the digits for my calculator program
int arrangeDigits (int currentNum, int numIn)
{
    if (currentNum < 100000000)
    {
        currentNum *= 10;
        currentNum += numIn;
    }
    return currentNum;
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}