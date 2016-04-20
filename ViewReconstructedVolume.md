# Introduction #

After running RabbitCT you might want to have a look at your beautifully reconstructed rabbit. This page provides instructions on how to install and use [ImageJ](http://rsbweb.nih.gov/ij/) for this purpose.

Btw.: The applet on the [RabbitCT website](http://www5.informatik.uni-erlangen.de/Forschung/Software/RabbitCT/volumeviewer.html) also uses ImageJ.


# Details #

  1. Download the appropriate ImageJ package from their website.
  1. Install ImageJ. (Note for Windows users: we recommend to not install ImageJ in the Program Files folder as ImageJ writes its config file in its install directory and for writing to Program Files you need admin rights)
  1. Now you can open RabbitCT result files:
    * Click: `File` -> `Import` -> `Raw`
    * Select your volume file.
    * In the "Import..." dialog choose the following values:
      * Image type: 32-bit Real
      * Width, Height, Number of images: 512 (or your volume size)
      * Offset and Gap: 0
      * Little-endian: true
      * Other checkboxes: false

# Troubleshooting #
If you get OutOfMemory errors you may have to increase the memory of your Java Virtual Machine (JVM). Open ImageJ.cfg in your ImageJ directory and adjust the parameter -Xmx (or add it).
Here is an example of how the config file could look like (the JVM is allowed to consume up to 3G of memory):
```
.
C:\Program Files\Java\jdk1.6.0_24\bin\javaw.exe
-Xmx3098m -cp ij.jar ij.ImageJ
```


# References #
`[1]` ImageJ website - http://rsbweb.nih.gov/ij/