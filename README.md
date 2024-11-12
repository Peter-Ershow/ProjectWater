# ProjectWater🌖
I use

[![Used](https://skillicons.dev/icons?i=unreal)](https://www.unrealengine.com/en-US)
[![Used](https://skillicons.dev/icons?i=cpp)](https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-programming-and-scripting)
[![Used](https://skillicons.dev/icons?i=rider)](https://www.jetbrains.com/lp/rider-unreal/)
[![Used](https://skillicons.dev/icons?i=discord)](https://discord.gg/VctsYqtd)

## Assets 🕷️🕷️🕷️
https://drive.google.com/drive/folders/1Q6l-vot1sTp-YEkimqEp29frsfUms3L6?usp=sharing

## Working with code
You can work with files using GitHub Desktop. Download and fetch changes regularly, and always create pull requests to avoid conflicts in files. The process is: create a branch, push to that branch, and a pull request option will appear on the page. Create the pull request and then merge it.

## CI/CD 🐿️
# How to Work with Perforce

## Key Working Folders and Files

![GIF](https://media0.giphy.com/media/W8ndsoHqSCGBUzHb4a/giphy.gif?cid=7941fdc6fwc5973n3tte3j2p6l60a38n2ngzae8ly34xqko7&ep=v1_gifs_search&rid=giphy.gif&ct=g)

### `Content`
This folder contains everything unrelated to code, such as blueprints, assets, textures, inputs, sounds, and other resources. **If something breaks here, it's critical** – let the team know immediately (e.g., on Telegram). Keep an eye on this folder as you can’t easily verify its contents with code checks, and rebuilding from scratch could be challenging.

### `Source`
This folder holds all the code, main classes, etc. Currently (and hopefully permanently), it mirrors the `Common` folder in `Content`. Follow this structure as a best practice and avoid creating new directories or files in other locations without a team decision. Moving content locally is much easier.  

- **Actors**: These are subclasses of interfaces that add functionalities to characters, like the ability to play music or consume food. See the UML Diagram:
  ![UML](https://github.com/Peter-Ershow/ProjectWater/uml.png)

  *(Subject to change)*

- **Interfaces**: Interface definitions.

- **Characters**: A folder for the main character and other character types (e.g., NPCs). Contains AI logic and decision branches as well.

- **Components**: Components that can be attached to actors or characters. Think of these as customizable functional modules that you can swap in and out.

- **UI**: Minimal code for UI, including a notifier and inventory manager.

- **Other Folders**: Configuration files are here, but they are mostly empty for now. Base configurations are set through blueprints.

---

## Step-by-Step Workflow with Perforce (Using P4V)

![GIF](https://media3.giphy.com/media/b6iVj3IM54Abm/giphy.gif?cid=7941fdc6xn9wek6k7644itnid0tu4lbvc3bkk74p3hh2rufa&ep=v1_gifs_search&rid=giphy.gif&ct=g)

1. **REFRESH!**  
   Familiarize yourself with the icon meanings: [Perforce Icons](https://www.perforce.com/manuals/p4v/Content/P4V/using.icons.html).

2. **Get Latest Data from Depot**  
   Fetch the latest data by right-clicking the root folder and selecting **Get Latest Revision**.
3. **Handle Errors**  
   If an error appears, read the message carefully. Most of the time, it’s due to unimportant binaries. However, if it’s related to files in `Source` or `Content`, check **Depot** for recent changes and look at the **History** for context.

4. **Work on Files**  
   Once files are fetched, open the engine or Rider and work as usual. For C++ files, **ALWAYS check out the file in P4V**. For files in Unreal Editor (such as those with icons and scenes), ensure **Revision Control** is connected.

5. **File Checkouts and Changes**  
   - Before starting work, **make sure all files you need are checked out and not locked by anyone else.**
   - When adding new code files, mark them as **Mark for Add** in P4V, then submit. Verify in Depot afterward.
   - When deleting files, mark them as **Mark for Delete** and verify in Depot as well.

6. **End-of-Work Checklist**  
   Check Depot to ensure everything is properly synced.

---

## Step-by-Step Workflow with Perforce (Using Unreal Editor)

1. If working with assets in the `Assets` folder (which no one else should touch), you can connect to Revision Control from Unreal Editor.  
   - Server: `ssl:207.154.252.182:1666`
   - User: your Perforce username
   - Workspace: your workspace name

2. In the **Editor Preferences** under `Edit -> Editor Preferences`, ensure these settings:

3. **Submit Files**  
   After working, submit all files from the bottom-right menu. Verify in Depot to ensure everything is in order.

4. **Bonus Setup**  
   To enable `Use P4 Config` to auto-fill information:
   1. Create a `.p4config` file in the root of your main disk with:
      ```
      P4PORT=ssl:207.154.252.182:1666
      P4CLIENT=your_workspace_name
      ```
   2. Go to `My Computer -> Properties -> Advanced System Settings -> Environment Variables`.  
      Add a new **System Variable**:  
      - **Name**: `P4CONFIG`  
      - **Value**: Path to `.p4config`
   3. Restart Unreal and P4V (or restart your computer).

---

## Useful Links

1. [Using Perforce as Source Control for Unreal Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-perforce-as-source-control-for-unreal-engine)
2. [YouTube Guide 1](https://www.youtube.com/watch?v=7PRo8gK6SNM)
3. [YouTube Guide 2](https://www.youtube.com/watch?v=0HlLccLl9oI)
4. [YouTube Guide 3](https://www.youtube.com/watch?v=YKMDdtX-8gM)

---

## Troubleshooting

Document any recurring issues here for future reference.

1. **Failed to load outer or Missing Parent class errors**: This usually means a new file was added without updating binaries.  
   - Delete `Binaries`, `Saved`, and `Intermediate` folders, then right-click the `.uproject` file and select **Generate Visual Studio project files**.


Name of our runner group is IronFleet, but it's default so just press Enter
Name your ship however you want
When you run it you can either merge PR, or, if already, just rerun the job from the main repository page
