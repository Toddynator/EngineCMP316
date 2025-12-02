# Libraries Used:
-	SDL3 [ZLib]: https://github.com/libsdl-org/SDL 
-	Jolt [MIT]: https://github.com/jrouwe/JoltPhysics 
-	Mesher [MIT]: https://github.com/cgerikj/binary-greedy-meshing
-	SoLoud [ZLib] / [LibPNG]: https://solhsa.com/soloud/ 
-	ImGui [MIT]: https://github.com/ocornut/imgui/tree/docking 
-	ImGuizmo [MIT]: https://github.com/CedricGuillemet/ImGuizmo 
-	ImGuiFileDialog [MIT]: https://github.com/aiekick/ImGuiFileDialog/tree/master 
-	Stb_image [MIT] / [Public Domain]: https://github.com/nothings/stb 
-	Entt [MIT]: https://github.com/skypjack/entt 
-	OBJ Loader [MIT]: https://github.com/Bly7/OBJ-Loader

# Getting Started:
## Key Features:
-	3D Rendering (DirectX11)
-	Entity Management (ECS)
-	Scene Management
-	Resource/Asset Management
-	Input Management
-	Window Management
-	Audio
-	Physics
-	Serialization
-	Level Editor
-	ECS Systems: Level Editor, Rendering, Physics, Transforms, Camera.
-	Minimal Integrated Voxel Support
-	Model Loading for ‘Obj’ and MagicaVoxel ‘Vox’ Files.

## Intended Use:
A 3D only engine, with features to support but not restrict to voxel game development. 
This engine is intended for a data orientated design approach to developing games. Compared to typical Game objects utilized in other game engines, the entities are now purely an ID. Components are storage containers that contain no functionality. The mechanics of the game are instead defined in ECS systems which loop through all components of a particular type. 
The existing level editor is designed for easily creating the entities and for adding and removing components, the object tree has been implemented to support this, with every entity being created with a Hierarchy Component. Importantly, you do not need to create entities with a hierarchy component, although anything created with the level editor will be generated with one. For example, during runtime you may have a system that spawns bullets which won’t need hierarchal functionality. 
Best practice is to divide the components into the smallest meaningful units of data possible so that they represent one aspect of an entity. For example, a MovementComponent which represents intended direction, a SpeedComponent which represents speed, AccellerationComponent that represents change in speed over time and so on. This allows maximum reuseability between entities. An enemy and player entity could share all those components, but a bullet might only use the Speed Component and have a single forward direction that never changes. Looking at the existing ECS systems should give an idea of how to then loop through the components to add functionality.

## Quirks/Known Issues:
The camera in the Level Editor can sometimes jolt to the side and it can be hard to reorientate, in that case it may be best to just save your work and reboot the engine. Alternatively, if using the Example Game Project, pressing on the scene to ensure ImGui isn’t selected then pressing ‘Escape’ should reload the Level Editor. 
Sometimes Cmake will complain when trying to build, typically, cleaning then rebuilding the project will fix this, although sometimes simply just trying the build again will solve it. Otherwise open the CmakeList.txt that is in the root directory with visual studio 2022 and press build.
Additionally, ensure that the executable is selected.
 
## Installation:
The Engine has not been tested with IDEs other than Visual Studio 2022. However, it should be possible to use other IDEs thanks to Cmake.
Cmake with Visual Studio 2022:
1.	Download or Clone the Repository https://github.com/Toddynator/EngineCMP316/tree/Voxels
2.	Open Visual Studio 2022
3.	Press ‘Open a Local Folder’ and open the root directory of the Engine Directory.
4.	VS2022 should detect the CmakeList file and begin building the Cmake Project automatically. Wait until Cmake Generation has finished. Alternatively, if it is not building, open the CMakeList.txt file in the root directory with VS2022 and Build.
5.	Select the Engine Executable and Build the executable.
6.	The Engine should now be running with the Demo Game Project.

## Getting Started
### Testing out the Demo Game Project:
Once the engine is running, I recommend pressing F11 to fullscreen the window, then in the top left press the Save & Load tab and click Load. A file dialog should open. 
Once the dialog is open, I recommend selecting the Placeholder level then pressing ‘ok’.
Feel free to then add objects via the Scene GameObject Tree window on the left-hand side. Selecting any of the objects by either clicking on them in the object tree or clicking on them in the scene will allow you to edit its components in the Object Inspector on the Right-Hand side.
Feel free to Save the level using the same tab that you used to load the level.
If you then press on the scene (Ensure an ImGui window isn’t selected), press ‘L’ and that should run the Placeholder.level file that was just open. Pressing ‘Escape’ will take you back to the level editor. 
Note the File Explorer in the bottom Left has no functionality as of now, although it could be utilized by developers if they inherit the existing level editor system and make their own level editor scene.

### Starting a new Game Project:
1.	Delete the Game Folder in the Engine Root Directory
2.	Open the CmakeList.txt file in the root directory and build. This should generate a new Game Folder.
3.	Ensure the Engine executable is selected. Run the engine, a window with a level editor should open possibly with some Starting Objects. The Placeholder.Level file should also be available to load to test the engine deserializes correctly.
 
If Cmake did not generate a new Game Folder or the generated folder does not work, then use the demo game project and modify / remove the existing files.

### Registering a New Scene:
SceneManager supports 2 methods of registering a scene, RegisterScene() with just an Id as parameter will take the templated Scene Class and run that without loading any files. RegisterScene() with an ID and string will load a ‘.level’ file of that name from the data/Levels folder. 
In Game.h there is an enum you can use to index your scenes, add an enum for your new scene. Then in the Initialize() function of your Game class, call RegisterScene with the new ID, if you’ve made a new scene class, use that, otherwise you can use the ECSScene Class.
 
Modify the ID passed into RequestSceneChange() to use your new scenes ID or if ithere is no function call then create one. You can request a scene change at any time in your Game by accessing engineContext.sceneManager and callinng the RequestSceneChange() function. The only difference between the LevelEditorScene and a normal ECS scene is that there is a Level Editor ECS system pushed into the scene’s system vector.  

### Creating a New Scene:
1.	Create a new class for your Scene in the Game Folder.
2.	Inherit ECSScene (Make sure to use the engine’s namespace).
3.	Create a Constructor that takes EngineContext as reference.
4.	In the Constructor call the ECSScene constructor and pass in context.
5.	Push to the systems vector any additional systems you have made for your scene.
Feel free to implement the Initialize() as well, e.g. for playing audio or creating default entities. Remember to call ECSScene’s Initialize() function. For example, push the PhysicsSystem that is under the Engine Namespace. Now any entities with RigidComponents will be updated by that system.
6.	In the Game::Initialize() function in your Game.cpp file you can now Register the Scene with your class using an Integer ID of your choice and load any Level file that you’ve created in the LevelEditor. Alternatively, if your scene is manually created and you don’t want to use the editor then you can just use the ID.
 
I recommend using the ECSHelper class if you choose to manually create a scene, it handles adding the default components required for Entities to operate in the scene Hierarchy, as well as modifying hierarchal relationships.
NOTE: Any levels created with the level editor will go to the bin folder. Navigate to the version you are running and copy from the Data/Level folder the level file you’ve created, then paste it in the data folder in the root directory. The Working Directory that the files are saved to is placed with the executable, but rebuilding the project copies the data folder from the root directory.

### Creating & Reflecting a New Component:
I recommend looking at the Components class in the Engine folder for reference, in particular the Components.cpp file which has extensive comments on how to reflect components.
1.	Make your own Component Class in the Game Folder.
2.	Create a component of your choice (e.g. a PlayerComponent so that you can differentiate the player from other entities).
3.	Create a Function for reflecting your components. OPTIONALLY you can create a class that calls the function in its constructor and can define a static object of that class so that it automatically reflects your components on compiling.
4.	In the cpp file, reflect your component. You can use the recommended reflection syntax provided in the engine component file. 
5.	Your component should now show in the editor, any variable that you append the Editor Trait will appear and if the engine has implemented controls for that data type, it should be controllable in the Editor UI.  
NOTE: Adding or Removing components as well as changing the order of variables is safe and the save / level files should still be forward / backward compatible, however if you add or remove any variables then the file will no longer be compatible, and you will be unable to load it.
You can update your level/save files to use the new component by modifying the reflection object for your component and appending the NOT_DESERIALIZED or NOT_SERIALIZED trait to your variables. You can then update all your level/save files to the latest version of serialization. Highly recommend making a copy of your level / save files before attempting to update them.
If you add a variable: Append NOT_DESERIALIZED then load all the files and serialize them. Remove the trait and your level files should now all be updated to the latest version.  
 
If you want to remove a variable: First Append NOT_SERIALIZED, then load all the files and serialize them. Remove the variable and all its reflection information. All your level files should now be updated to the latest version.
 
### Creating a New System:
1.	Create a new Class in your Game Folder that inherits the Engine ‘System’ Class. Remember to use the Engine Namespace to access it. The constructor Signature must at minimum have the same parameters as the System class constructor so that it can then call the System constructor. You can add additional paramaters as needed.  You will need to override all pure virtual functions in the System class even if it’s an empty implementation. 
2.	For any engine systems that you want to maintain a pointer to, you should set them in the constructor using the EngineContext.
3.	In your games scene, you can then push your new system to the levels system vector.
 
For implementing functionality, look at the existing systems and or Entt Documentation for how to interact with Entities. For Example:
  
### Creating a Level in the Level Editor:
1.	If you’ve managed to launch the editor / registered the level editor scene and changed the scene to the editor, then you can continue with the following steps. Otherwise refer to ‘Registering a New Scene’.
2.	On the left-hand side you should see the Scene Tree, select the ‘SCENE ROOT’ and then press ‘Add’ which should be along the top of the hierarchy. 
3.	Select the newly created entity in the tree.
4.	On the right-hand side you should see the Object Inspector, if you’ve selected the entity, then you should see an option to add a component. Select it to open a dropdown and press any component.
5.	In the Top Left of the screen, you should see a Save & Load tab, press this and click ‘Save’. Choose a destination and name, preferably a Level Folder.
6.	Once you’re happy with your scene, you can now reload it at any time via the Save & Load tab OR by registering it as a scene. Ensure you copy it into the root directory data folder if you want to keep the level file after rebuilding.