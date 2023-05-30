#include "AblazePch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Ablaze/Core/Application.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

//#include "Ablaze/Platform/OpenGL/imgui_impl_glfw.h"
//#include "Ablaze/Platform/OpenGL/imgui_impl_opengl3.h"

namespace Ablaze {
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		ABLAZE_PROFILE_FUNCTION()

		//����Imgui������
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		//���Imgui�����ĺ�ϵͳ֮��I/O�Ľӿ�
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
															// ʹ��imgui���ڿ����Ƴ���GLFWwindow��
		
		ImGuiStyle& style = ImGui::GetStyle();
		//�����ӿں����ǵ���WindowRounding/WindowBg��ʹƽ̨���ڿ������볣�洰����ͬ
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		/*Platform Binding:��GLFW�Ŀ⣬���������簴�����룬�������֮����¼��������
		true��������Щ����Ҫ��Ҫ��callback��imgui*/
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		//Renderer Binding:imgui��֪�����׳�ʼ��ʲôGL
		ImGui_ImplOpenGL3_Init("#version 430");
	}
	void ImGuiLayer::OnDetach()
	{
		ABLAZE_PROFILE_FUNCTION()

		//�����Ӵ���VAO��buffer��shader��shader program
		ImGui_ImplOpenGL3_Shutdown();
		//ȡ��Callback��
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::Begin()
	{
		ABLAZE_PROFILE_FUNCTION()

		//����VertexArray��Shader��Buffer��Attributes��һϵ����Ⱦ��صı�ҪDevice Object
		ImGui_ImplOpenGL3_NewFrame();
		//�趨�Ӵ���С����ǰDeltaTime���������λ�ã���갴��������꣬�����ֱ��¼���IO��ص���Ϊ��
		ImGui_ImplGlfw_NewFrame();
		//��������Ӵ�����Ⱦ֡��IO��FPS��DeltaTime�����壬��ҷ�������ͣ���Ӵ�����Ⱦ���ݶ�д���������յ�
		ImGui::NewFrame();
	}
	void ImGuiLayer::End()
	{
		ABLAZE_PROFILE_FUNCTION()

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		//�ܺ����ݣ��ύ��Renderer������DrawData����Ⱦ����꣬�ܺ���Ⱦ�Ķ������ݺͶ���Indices����
		ImGui::Render();
		//��֮ǰ���ܵ������ϴ���Renderer�����ҽ�����Ⱦ
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}