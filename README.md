# Birdiary - station - ESP32 
Nowadays, more species are threatened with extinction than ever before in the human era.
Especially breeding birds are considered endangered. 
At the same time, it is difficult to obtain sufficient data not only to raise awareness for this situation, but also to gain a better understanding and to develop potential countermeasures.
The Birdiary project demonstrates how a citizen science based biodiversity monitoring for birds, using an automated and easy-to-use multi-sensor feeder, can look like. 
A smart bird feeder including an environmental sensor, a microphone, as well as a balance and a camera in a wooden case which identifies the type of visiting birds using AI and publishes the recognized species, including all further collected data on an open access website. 
The station can be reproduced by anyone at an affordable price in a Do-It-Yourself format, making citizens a key contributor to biodiversity monitoring.

This repository contains the code to run the Birdiary script on ESP32 (suggested microcontroller: [Seeed Studio XIAO ESP32S3 Sense](https://www.seeedstudio.com/XIAO-ESP32S3-Sense-p-5639.html)). All further Information to reproduce the code and the station can be found in the [GetStarted.md](/GetStarted.md) Manual. 
All the collected data is send to the [Birdiary Webserver](https://www.wiediversistmeingarten.org/view).
The repositories are currently still under development, the code for the operation of the feeder as well as for the web server are continuously updated. 

Birdiary is a project which was launched by a group of students at the Institute for Geoinformatics at the University of Münster. 
If you got any questions contact us via: [info@wiediversistmeingarten.org](mailto:info@wiediversistmeingarten.org).

Any further information you can find via: [birdiary.de](https://www.wiediversistmeingarten.org/). 

## Run your own ESP32 Birdiary 
To run the Birdiary script on a ESP32 follow the [GetStarted.md](/GetStarted.md) Manual. 
Unfortunately, there are currently only instructions for the hardware design of the birdhouse based on the Raspberry Pi ([Birdiary Manual](https://docs.google.com/document/d/1ItowLull5JF3irzGtbR-fCmgelG3B7DSaU1prOeQXA4/)). Nevertheless, you can use these instructions as a starting point if you also want to build a birdhouse adapted for ESP32. It is likely that adjustments will be necessary to attach the sensors. 

## How to Contribute
Thank you for considering contributing to Birdiary. 
Birdiary is an open source project, and we love to receive contributions from our community — you!
 
There are many ways to contribute, from writing tutorials or blog posts, improving the design of the station, submitting bug reports and feature requests or writing code which can be incorporated into Birdiary itself.
 
We use GitHub to host code, to track issues and feature requests, as well as accept pull requests.
 
This Repository comprises the code and issues for the software to operate the Birdiary script on a ESP32. 
Which means, if you want to contribute to the Birdiary code running on a ESP32, please contribute here. 
If you want to contribute to the Birdiary code running on a Raspberry Pi and if you want to contribute in terms of the hardware or design of the station you can share your ideas in the [Birdiary Station Repository](https://github.com/Birdiary/station). 
Whereas when you want to contribute to the webserver (website), please use the [Birdiary Webserver Repository](https://github.com/Birdiary/webserver). 

### Reporting bugs
If you find a security vulnerability, do NOT open an issue. Send an email to the Birdiary team via [info@wiediversistmeingarten.org](mailto:info@wiediversistmeingarten.org) instead.
 
If you encounter a bug, check if the bug has already been reported here as [issue](https://github.com/tnier01/BirdiaryStationESP32/issues). If the bug has not been reported, you can report the bug as new [issue](https://github.com/tnier01/BirdiaryStationESP32/issues/new). For a bug, please add the label "bug".
 
When filing an issue, make sure to answer these four questions:
> 1. Which hardware components (e.g. microcontroller, sensors) are you using?
> 2. Which software components (e.g. IDE, libraries, packages) are you using?
> 3. What did you do?
> 4. What did you expect to see?
> 5. What did you see instead?
 
### Suggest Feature
If you wish a special feature, feel free to add it as new [issue](https://github.com/tnier01/BirdiaryStationESP32/issues/new). Here, please add the label "enhancement". We appreciate any suggestions.
 
### Code contributions
Besides reporting bugs or suggesting features, we really appreciate code contributions. We suggest contributing through forking and pull-requests. A guideline how to fork a project and create a pull request can be found in the [Contribution to Projects Guidelines](https://docs.github.com/en/get-started/quickstart/contributing-to-projects). 
 
We review pull requests on a regular basis and give feedback or merge them directly into our main repository.
 
### Validating an issue or pull request
You can also contribute by merging a pull request into your local copy of the project and testing the changes. Add the outcome of your testing in a comment on the pull request.
 Further, you can validate an issue or add additional context to an existing issue.