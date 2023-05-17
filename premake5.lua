workspace "Ablaze-Engine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    startproject "Ice"
    
outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--创建一个相对于解决方案的目录
IncludeDir={}
IncludeDir["glfw"]="Ablaze/vendor/glfw/include"
IncludeDir["Glad"]="Ablaze/vendor/Glad/include"
IncludeDir["imgui"]="Ablaze/vendor/imgui"
IncludeDir["glm"]="Ablaze/vendor/glm"

include "Ablaze/vendor/glfw"
include "Ablaze/vendor/Glad"
include "Ablaze/vendor/imgui"
--include "Ablaze/vendor/glm"

project "Ablaze"
    location "Ablaze"
    kind "SharedLib"
    language "C++"

    targetdir("bin/"..outputdir.."/%{prj.name}")
    targetdir("bin-int/"..outputdir.."/%{prj.name}")
    -- 生成.pch预编译头文件
    pchsource "%{prj.name}/res/pch/AblazePch.cpp"
    -- 使用生成的预编译头文件
    pchheader "pch/AblazePch.h"

    files
    {
        "%{prj.name}/res/**.h",
        "%{prj.name}/res/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/res",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}"
    }

    links{
        "glfw",
        "opengl32.lib",
        "Glad",
        "imgui"
    }
    filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"ABLAZE_PLATFORM_WINDOWS",
			"ABLAZE_BUILD_DLL",
            "ABLAZE_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"

		}

	
	postbuildcommands{
		("{COPY} %{cfg.buildtarget.relpath} \"../bin/" ..outputdir.. "/Ice/\"")
	}

	filter "configurations:Debug"
		defines "ABLAZE_DEBUG"
		--runtime "Debug"
        buildoptions "/MDd"--指定为多线程动态运行库，d代表Debug模式
		symbols "On"

	filter "configurations:Release"
		defines "ABLAZE_RELEASE"
		--runtime "Release"
        buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ABLAZE_DIST"
		-- runtime "Release"
        buildoptions "/MD"
		optimize "On"



project "Ice"
    location "Ice"
    kind "ConsoleAPP"
    language "C++"
    staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/res/**.h",
        "%{prj.name}/res/**.cpp"
    }
    
    includedirs
    {
        "Ablaze/vendor/spdlog/include",
        "Ablaze/res",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}"
    }
        
    links
    {
       "Ablaze"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
    
        defines
        {
           "ABLAZE_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "ABLAZE_DEBUG"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "ABLAZE_RELEASE"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "ABLAZE_DIST"
        runtime "Release"
        optimize "On"