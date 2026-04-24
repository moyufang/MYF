# Menu
## File Menu
### Pack

Blender's packing system is selective. It only recognizes and embeds files that it considers **resources**[](https://www.bookstack.cn/read/blender-4.4-en/15c8839d37f73e0b.md). According to the official Blender documentation, these are:

- **Image Textures** (e.g., `.jpg`, `.png`, `.tif`, `.psd`)
- **Fonts** (e.g., `.ttf`, `.otf`)
- **Linked Libraries** (other `.blend` files)

others like “mp4, md, txt" cannot be embedded into a `.blend` file

How to Store "Readme" or Metadata in a `.blend` File
- Custom Properties: You can attach a custom property to any object, mesh, or even the scene itself. This property can be a string of text, an integer, or a float.
- Text Object in the scene
- The Text Editor: Blender has a built-in Text Editor that is perfect for writing and storing lengthy notes, instructions, or even Python scripts within the `.blend` file.

# Hot keys
