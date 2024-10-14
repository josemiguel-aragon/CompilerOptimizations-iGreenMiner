# CompilerOptimizations-iGreenMiner
Measurement scripts, benchmark applications and experiment results on the impact of compiler optimization on iOS applications.

For our evaluation, we have selected two applications from each of the following three popular categories:

### Disk Operations

This category focuses on applications that perform intensive disk operations, common in smartphone usage. Two iOS apps were selected:

- **SQLitePolybench (SQLPoly)**: A Swift app that combines SQLite and Polybench benchmarks, running a loop 10,000 times with database insertions and updates.
- **FileManagement (FileMng)**: A Swift app performing 1,000 file write and delete operations using the `FileManager` library.

### Deep Learning Models

This category covers apps using pre-trained deep learning models for inference, which is increasingly common in mobile apps:

- **ClassificationInference (ClassInf)**: A Swift app performing 10,000 image classifications using the `ImageClassifier` model, categorizing electronics into 11 classes.
- **SegmentationInference (SegInf)**: A Swift app that runs 1,000 image segmentations on cat images using the `DeepLab` model.

### Video Games

This category benchmarks mobile games, a fast-growing segment of the app market:

- **FlappyFlyBird (FlyBird)**: A Swift-based clone of *Flappy Bird* using the SpriteKit framework, where 5,000 player objects are updated, and the session ends after 3,000 frames.
- **GhostRun (GhostR)**: A Swift game inspired by the *Dinosaur Game*, featuring 25,000 player instances with identical game sessions repeated for benchmarking.
