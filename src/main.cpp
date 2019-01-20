#include "engine.h"

#include <iostream>

//function to parse command line arguments and get name of launch file
bool GetLaunchFile(CommandLineArgs& args, std::string & launchFile);

bool ParseCommandLine(int argc, char* argv[], CommandLineArgs &args);

int main(int argc, char * argv[]) {

    CommandLineArgs args;
    if (!ParseCommandLine(argc, argv, args))
        return 0;

    std::string launchFile;

    if (!GetLaunchFile(args, launchFile)) {
        std::cerr << "Error getting paramter values " << std::endl;
        return -1;
    }

    try {
        Engine engine(launchFile, args);
        std::cout << std::endl << "Running Engine! " << std::endl;
        engine.Run();
        std::cout << std::endl << "Engine Done Running. Now exiting! " << std::endl;
    } catch (std::string & err) {
        std::cerr << std::endl << "The engine crashed: " << err << std::endl;
        return -1;
    } catch (...) {
        std::cerr << std::endl << "The engine crashed! Error is unknown " << std::endl;
        return -1;
    }

    return 0;
}

bool GetLaunchFile(CommandLineArgs &args, std::string & launchFile) {
    if (args.m_FLAGS & CommandLineFlags::LAUNCHFILE) {
        std::cout << "Setting config file to: " << args.m_launchFile << std::endl;
        launchFile = args.m_launchFile;
    } else {
        launchFile = "launch/DefaultConfig.txt"; //default launch file
    }

    return true;
}

bool ParseCommandLine(int argc, char* argv[], CommandLineArgs & args) {
    args.m_FLAGS = 0;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-h") == 0) {//display information about application and command line arguments, exit the application
            //whoot
        } else if (std::strcmp(argv[i], "-l") == 0) {//launch file
            args.m_FLAGS |= CommandLineFlags::LAUNCHFILE;
            args.m_launchFile = std::string(argv[++i]);
#ifdef DEBUG
            std::cout << "Launch file: " << args.m_launchFile << std::endl;
#endif
        } else if (std::strcmp(argv[i], "-s") == 0) {//autoplay interval, s for speed
            args.m_FLAGS |= CommandLineFlags::AUTOPLAY;
            try {
                args.m_autoplayInterval = std::stof(argv[++i]);
            } catch (std::string & err) {
                std::cerr << "Invalid autoplay interval: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                return false;
            }
            #ifdef DEBUG
            std::cout << "Autoplay interval: " << args.m_autoplayInterval << std::endl;
#endif
        } else if (std::strcmp(argv[i], "-n") == 0) {//number of objects
            args.m_FLAGS |= CommandLineFlags::NUM_OBJECTS;
            try {
                args.m_numObjects = std::stoi(argv[++i]);
            } catch (std::string & err) {
                std::cerr << "Invalid number of objects: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                return false;
            }
            #ifdef DEBUG
            std::cout << "Number of objects (along edge): " << args.m_numObjects << std::endl;
#endif
        } else if (std::strcmp(argv[i], "-a") == 0) {//set ambient level
            args.m_FLAGS |= CommandLineFlags::AMBIENT_LEVEL;
            int firstComma = std::string(argv[++i]).find(',');
            if (firstComma != std::string::npos) {
                std::string ambientLight(argv[i]);
                try {
                    args.m_ambientLevel.r = std::stof(ambientLight.substr(0, firstComma));
                } catch (std::string & err) {
                    std::cerr << "Invalid ambient light level: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
                int secondComma = ambientLight.find(',', firstComma + 1);
                try {
                    args.m_ambientLevel.g = std::stof(ambientLight.substr(firstComma + 1, secondComma - firstComma));
                } catch (std::string & err) {
                    std::cerr << "Invalid ambient light level: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
                try {
                    args.m_ambientLevel.b = std::stof(ambientLight.substr(secondComma + 1, ambientLight.size() - secondComma));
                } catch (std::string & err) {
                    std::cerr << "Invalid ambient light level: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
            } else {
                try {
                    args.m_ambientLevel.r = std::stof(argv[i]);
                } catch (std::string & err) {
                    std::cerr << "Invalid ambient light level: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
                try {
                    args.m_ambientLevel.g = std::stof(argv[++i]);
                } catch (std::string & err) {
                    std::cerr << "Invalid ambient light level: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
                try {
                    args.m_ambientLevel.b = std::stof(argv[++i]);
                } catch (std::string & err) {
                    std::cerr << "Invalid ambient light level: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
            }
            #ifdef DEBUG
            std::cout << "Ambient light: " << args.m_ambientLevel.r << ", " << args.m_ambientLevel.g << ", " << args.m_ambientLevel.b << std::endl;
#endif
//        } else if (std::strcmp(argv[i], "-v") == 0) {//set vertex shader
//            args.m_FLAGS |= CommandLineFlags::VERTEX_SHADER;
//            args.m_vertexShaderPath = std::string(argv[++i]);
//        } else if (std::strcmp(argv[i], "-f") == 0) {//set fragment shader
//            args.m_FLAGS |= CommandLineFlags::FRAGMENT_SHADER;
//            args.m_fragmentShaderPath = std::string(argv[++i]);
        } else if (std::strcmp(argv[i], "-w") == 0) {//set window size/fullscreen
            args.m_FLAGS |= CommandLineFlags::WINDOW_SIZE;
            int commaPos = std::string(argv[++i]).find(',');
            if (commaPos != std::string::npos) {//one argument, height/width separated by comma
                std::string dimensions(argv[i]);
                try {
                    args.m_windowSize.x = std::stoi(dimensions.substr(0, commaPos));
                } catch (std::string & err) {
                    std::cerr << "Invalid height: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
                try {
                    args.m_windowSize.y = std::stoi(dimensions.substr(commaPos + 1, dimensions.size() - commaPos));
                } catch (std::string & err) {
                    std::cerr << "Invalid width: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
            } else {//height then width or fullscreen
                if (std::strcmp(argv[i], "full") == 0 || std::strcmp(argv[i], "fullscreen") == 0) {
                    args.m_FLAGS |= CommandLineFlags::FULL_SCREEN;
                    args.m_windowSize = glm::uvec2(0, 0);
                } else {
                    try {
                        args.m_windowSize.x = std::stoi(argv[i]);
                    } catch (std::string & err) {
                        std::cerr << "Invalid height: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                        return false;
                    }
                    try {
                        args.m_windowSize.y = std::stoi(argv[++i]);
                    } catch (std::string & err) {
                        std::cerr << "Invalid width: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                        return false;
                    }
                }
            }
            if (args.m_windowSize.x < 0 || args.m_windowSize.y < 0) {
                std::cerr << "Invalid menu dimensions: " << args.m_menuSize.x << 'x' << args.m_menuSize.y << "!" << std::endl;
                std::cerr << "Now exiting!" << std::endl;
                return false;
            }
            #ifdef DEBUG
            std::cout << "Window size: " << args.m_windowSize.x << 'x' << args.m_windowSize.y << std::endl;
#endif
        } else if (std::strcmp(argv[i], "-m") == 0) {//set menu size
            args.m_FLAGS |= CommandLineFlags::MENU_SIZE;
            int commaPos = std::string(argv[++i]).find(',');
            if (commaPos != std::string::npos) {//one argument, height/width separated by comma
                std::string dimensions(argv[i]);
                try {
                    args.m_menuSize.x = std::stoi(dimensions.substr(0, commaPos));
                } catch (std::string & err) {
                    std::cerr << "Invalid height: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
                try {
                    args.m_menuSize.y = std::stoi(dimensions.substr(commaPos + 1, dimensions.size() - commaPos));
                } catch (std::string & err) {
                    std::cerr << "Invalid width: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
            } else {//height then width
                try {
                    args.m_menuSize.x = std::stoi(argv[i]);
                } catch (std::string & err) {
                    std::cerr << "Invalid height: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
                try {
                    args.m_menuSize.y = std::stoi(argv[++i]);
                } catch (std::string & err) {
                    std::cerr << "Invalid width: " << argv[i] << std::endl << "Now exiting!" << std::endl;
                    return false;
                }
            }
            if (args.m_menuSize.x <= 0 || args.m_menuSize.y <= 0) {
                std::cerr << "Invalid menu dimensions: " << args.m_menuSize.x << 'x' << args.m_menuSize.y << "!" << std::endl;
                std::cerr << "Now exiting!" << std::endl;
                return false;
            }
            #ifdef DEBUG
            std::cout << "Menu size: " << args.m_menuSize.x << 'x' << args.m_menuSize.y << std::endl;
#endif
        } else if (std::strcmp(argv[i], "-o") == 0) {//set object file
            args.m_FLAGS |= CommandLineFlags::OBJ_FILE;
            args.m_objectFilePath = std::string(argv[++i]);
            #ifdef DEBUG
            std::cout << "Objects file: " << args.m_objectFilePath << std::endl;
#endif
        } else {
            std::cerr << "Invalid parameter flag: " << argv[i] << std::endl;
            std::cerr << "Now exiting!" << std::endl;
            return false;
        }
    }
    return true;
}