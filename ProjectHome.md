RabbitCT provides an open platform for worldwide performance-comparison of backprojection implementations on different architectures using one specific, clinical, high resolution C-arm CT dataset of a rabbit. This includes a sophisticated benchmark interface, a prototype implementation in C++, and image quality measures.

This space for the project is quite new, so until we added more information here please visit the RabbitCT website: http://www.rabbitct.com/

Find out more [about RabbitCT](http://code.google.com/p/rabbitct/wiki/About).

**RabbitCT News**

02.01.2013 - Happy New Year 2013 and a very fast new algorithm: "Thumper"

Timo Zinßer recently submitted his optimized implementation using CUDA 4.2 and the KeplerI driven GTX 680. Now super fast in below 1 second for the 512 RabbitCT problem. Also he submitted the new fastest version for the 1024 problem which is reconstructed in about 6 seconds. Congrats to Timo for these results.
Further details on the implementation are t.b.a and will be published soon.

18.10.2012 - Algorithm Update: "RapidRabbit".

Eric Papenhausen from Stony Brook University again took the lead with an update on his CUDA 3.0 implementation now using a GTX 680. 3.0s for the 512 RabbitCT problem is the new record. (The original paper was submitted to HPIR Workshop 2011 — [download PDF](http://www.cs.sunysb.edu/~epapenhausen/publications/rabbitCT_hpir11.pdf))


15.06.2012 - New algorithm: "CERA".
> Matthias Elter and his group from Siemens AG were the first to submit results on a Kepler-based GPU. Expectedly, the new hardware was able to push the limits and set new records for the [512 cube](http://www5.cs.fau.de/research/projects/rabbitct/ranking/?ps=512) and [1024 cube](http://www5.cs.fau.de/research/projects/rabbitct/ranking/?ps=1024) RabbitCT problems. On a GTX 670 their CUDA 4.x-based implementation takes a mere 3.4s with the same error compared to runner-up Eric Papenhausen (which takes 5.5s on a GTX 480). The speed-up is in the range of the increased core and memory clock rates. Further, they also submitted results for the 1024 which they can solve in 36.1s on a Tesla C2070.
02.09.2011 - Added documentation: [ViewReconstructedVolume](ViewReconstructedVolume.md)
> After running RabbitCT you might want to have a look at your beautifully reconstructed rabbit. The new wiki page provides instructions on how to use ImageJ for this purpose.
09.08.2011 - RabbitCT mailing list.
> I thought it might be nice to have a mailing list for announcement of new submissions, news about the project, and discussions about future development. Therefore, I invite you to join [rabbitct@googlegroups.com](mailto:rabbitct@googlegroups.com) (send mail to [rabbitct+subscribe@googlegroups.com](mailto:rabbitct+subscribe@googlegroups.com), or use the [web interface](https://groups.google.com/group/rabbitct)).
03.08.2011 - New algorithm: "LionEatsRabbit".
> Wolfgang Wein of White Lion Technologies AG took the lead with the first submitted implementation on an AMD graphics card. He used OpenGL/GLSL on a Radeon HD 5870 and is just faster than Eric's RapidRabbit. The same 5.9something seconds for the [512 RabbitCT problem](http://www5.informatik.uni-erlangen.de/research/projects/rabbitct/ranking/?ps=512) with a minimal smaller error. (presented at Fully3D 2011 — [download PDF](http://ar.in.tum.de/pub/wein2011selfcal/wein2011selfcal.pdf))
29.05.2011 - New algorithm: "RapidRabbit".
> Eric Papenhausen from Stony Brook University took the lead with his CUDA 3.0 implementation on a GTX 480. 5.9s for the [512 RabbitCT problem](http://www5.informatik.uni-erlangen.de/research/projects/rabbitct/ranking/?ps=512) is the new record. (submitted to HPIR Workshop 2011 — [download PDF](http://www.cs.sunysb.edu/~epapenhausen/publications/rabbitCT_hpir11.pdf))
23.05.2011 - New algorithm: "RoadRunner".
> Christian Siegl from the Pattern Recognition Lab submitted another OpenCL implementation to the rankings of RabbitCT. The intention of this work was to investigate the portability of optimized OpenCL implementations across GPUs from different vendors. (published at SPIE Medical Imaging Conference 2011 — [download PDF](http://www5.informatik.uni-erlangen.de/Forschung/Publikationen/2011/Siegl11-OAV.pdf))
07.05.2011 - New algorithms: "fastrabbit" and "fastrabbitEX".
> Jan Treibig from the Regional Computing Center Erlangen (RRZE) submitted an optimized assembler implementation to the rankings of RabbitCT. On a high-end four-socket Nehalem-EX server (32 cores), fastrabbitEX was able to outperform all submitted implementations (8.9s). A dual-socket Westmere node (12 cores) was competetive with SpeedyGonzales on a NVIDIA Quadro FX5800 (14.8s). (submitted)
22.02.2011 - Source code version 1.0.3 released.
  * Added support for Prepare functions which are called after LoadModule and before the back-projection loop starts.
  * Added advanced variables to RabbitCtGlobalData.
  * Cleaned up the source code of RabbitCTRunner and added more comments.
  * Added LolaOpenMP module.
  * Added compiler optimization flags to CMakeLists.txt (for both runner and modules).
  * Added build- and run-test-linux.sh
> Get 1.0.3 from the [SVN](http://code.google.com/p/rabbitct/source/checkout) while it's hot!
22.02.2011 - Source code version 1.0.2 released.
> No functional changes. Applied patch to fix [issue #1](https://code.google.com/p/rabbitct/issues/detail?id=#1), added output of RMSE, and added a run test script.
21.02.2011: Updates to the ranking page.
> We made a few changes to the ranking pages to make them more comprehensible.
    * We added footnotes describing the individual columns so you don't have to look it up in the paper or code.
    * We replaced the variance (MSE) by the std. dev. (RMSE) because we think it is a more appropriate measure of the error.
    * We replaced the average time by the total runtime (measured in seconds) since this is what matters to the physician.
    * We removed GUPS from the ranking page. We don't consider GUPS to be a valid measure for comparing different implementations and hence see no use in providing it in the ranking. However, we believe that GUPS are a great measure for comparison of one implementation on different hardware or with different problem sizes. Hence, we included it on algorithm detail pages.
> Thanks to the community members who helped us improve R<span>abbit</span>CT with their suggestions.
18.02.2011 - Source code version 1.0.1
> No functional changes, but simpler directory structure. Also, added a build test script.
16.12.2010 - RabbitCT Open Source project released.
> Today we created a project at Google Code and uploaded the whole source code there. Over time we will also migrate the documentation and other content there. The first article in the [Wiki](http://code.google.com/p/rabbitct/w/list) describes how you can <a href='http://code.google.com/p/rabbitct/wiki/StaticLinking'>link R<span>abbit</span>CT statically against your module</a>. Check out the <a href='http://code.google.com/p/rabbitct/'>R<span>abbit</span>CT project page</a> at Google Code.
30.11.2010 - RabbitCTRunner source code will be released.
> We decided to release the last closed part of R<span>abbit</span>CT, the RabbitCTRunner, as open source software. We will eventually upload the whole source code to a public repository. If you want the full scoop now feel free to contact us. We welcome developers who want to get early access to the source code.
19.11.2010 - New algorithm "CLeopatra".
> Sebastian Schuberth from Zuse Institute Berlin submitted the first OpenCL implementation to the [rankings of RabbitCT](http://www.rabbitct.com/ranking).
30.09.2009 - R<span>abbit</span>CT published in Medical Physics.
> An article describing the idea and technical details of R<span>abbit</span>CT was published in the September issue of Medical Physics. You can find the abstract on the [Med. Phys. website](http://dx.doi.org/10.1118/1.3180956) or [download the manuscript](http://www5.informatik.uni-erlangen.de/Forschung/Publikationen/2009/Rohkohl09-TNR.pdf) (PDF).<br />If you use R<span>abbit</span>CT or the R<span>abbit</span>CT data set in a publication, please reference the article.<br />
> > "RabbitCT---an open platform for benchmarking 3D cone-beam reconstruction algorithms" Christopher Rohkohl, Benjamin Keck, Hannes G. Hofmann and Joachim Hornegger, Med. Phys. 36, 3940 (2009), DOI:10.1118/1.3180956<br />
> > ([View BibTeX](http://univis.uni-erlangen.de/prg?search=publications&id=90975790&show=bibtex))