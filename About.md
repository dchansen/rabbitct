# About RabbitCT #

Fast 3D cone beam reconstruction is mandatory for many clinical workflows. For that reason researchers and industry work hard on hardware-optimized 3D reconstruction. Many reconstruction algorithms employ a backprojection step, which requires a projection of each voxel onto the projection data, data interpolation, and finally a weighted update of the voxel value. In terms of runtime, backprojection commonly is the main factor of the processing pipeline. Hence, it also is in the focus of optimization in recent publications.

A crucial limitation, however, of these publications is that the presented results are not comparable, mainly due to variations in the data acquisition, preprocessing, or chosen geometry, caused by the lack of a common publicly available test dataset.

**RabbitCT provides** an open platform for worldwide performance-comparison of backprojection implementations on different architectures using one specific, clinical, high resolution C-arm CT dataset of a rabbit. This includes a sophisticated benchmark interface, a prototype implementation in C++ (LolaBunny), and image quality measures.

Sure enough you, too, want to know how your implementation compares against others on a fair basis. To implement your first backprojection module, please read the GettingStarted guide.

### Who is behind RabbitCT? ###
RabbitCT is a collaboration of the [Department of Neuroradiology](http://www.neuroradiologie.med.uni-erlangen.de/) and the [Pattern Recognition Lab](http://www5.cs.fau.de/) at the [Friedrich-Alexander-Universität Erlangen-Nürnberg](http://www.uni-erlangen.de/).