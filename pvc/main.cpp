//
//  main.cpp
//  pvc
//
//  Created by Jack on 6/9/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include <iostream>
#include <csignal>
#include "serve.hpp"
#include "fetch.hpp"

int main(int argc, const char * argv[]) {
	//if (argc < 2) return 1;
	messageLevel = -1;
	if (argc > 1 && std::string(argv[1]) == "server") {
		local_repo_info info;
		info.name = "TestServer";
		info.info.name = "TestServer";
		info.info.creator = "JackMacWindows";
		info.info.commits = 2;
		commit a, b;
		a.id = "b7760bf7";
		b.id = "e513a7cd";
		a.creator = "JackMacWindows";
		b.creator = a.creator;
		a.creator_signature = "bfdf5c923b474cf685f506df133f97e751f28f3d8dfec440b8cd94abb279f63f"; // just a test
		b.creator_signature = a.creator_signature;
		a.title = "First commit";
		b.title = "Second commit";
		a.timestamp = 1529191069;
		b.timestamp = a.timestamp + 2000;
		patch ap, bp;
		ap.filename = "README.md";
		ap.lineno = 1;
		ap.type = PATCH_NEW_FILE;
		ap.content = "# clicktheface\n\
A silly little game that I made in one day in JavaScript.\n\
Yeah, that's in JS.";
		bp.filename = "clicktheface.js";
		bp.lineno = 1;
		bp.type = PATCH_NEW_FILE;
		bp.content = "var score = 0;\n\
var audio = new Audio('onClickSound.mp3');\n\
// Game function\n\
function doClick() {  // When you click the picture\n\
	  score = score + 3;\n\
	  if (score > 1000) document.write(\"Why are you still clicking?\");\n\
	  document.getElementById(\"scoretext\").innerHTML = score;\n\
	  audio.play();\n\
}\n\
// Secrets\n\
function youWin() {  // Secret: Win the game\n\
	  var overkill = new Audio('winSound.mp3');\n\
	  overkill.play();\n\
	  score = score + 10000000000;\n\
	  document.write(\"you win! perfect!<br>\");\n\
	  document.write(\"With a score of: \" + score);\n\
}\n\
function youLose() {  // Secret: Get to 1000 points\n\
	  score = 999;\n\
	  audio.play();\n\
	  document.getElementById(\"scoretext\").innerHTML = score;\n\
}\n\
// Initializer\n\
function initAll(picname, scorename=\"Score: \", width=200) {\n\
	  var x = document.createElement(\"IMG\");\n\
	  x.setAttribute(\"src\", picname);\n\
	  x.setAttribute(\"width\", width);\n\
	  x.setAttribute(\"onClick\", \"doClick()\");\n\
	  document.body.appendChild(x);\n\
	  var y = document.createElement(\"P\");\n\
	  var z = document.createElement(\"P\");\n\
	  y.setAttribute(\"id\", \"name\");\n\
	  z.setAttribute(\"id\", \"scoretext\");\n\
	  document.body.appendChild(y);\n\
	  document.body.appendChild(z);\n\
	  document.getElementById(\"name\").innerHTML = scorename;\n\
	  document.getElementById(\"scoretext\").innerHTML = score;\n\
}";
		a.patches.push_back(ap);
		b.patches.push_back(bp);
		info.commits.push_back(a);
		info.commits.push_back(b);
		reposServed.insert(std::make_pair("TestServer", info));
		signal(SIGINT, stopServer);
		std::cout << "Running server\n";
		runServer();
		std::cout << "Server stopped\n";
	} else {
		std::string dir, url, repo;
		if (argc > 1) {
			std::cout << "Enter the URL: ";
			std::getline(std::cin, url);
			std::cout << "Enter the repo: ";
			std::getline(std::cin, repo);
			std::cout << "Enter the directory: ";
			std::getline(std::cin, dir);
		} else {
			dir = "/Users/jack/Downloads/pvc-testrepo";
			url = "127.0.0.1";
			repo = "TestServer";
		}
		std::cout << "Downloading...\n";
		fetchRepository(url, repo, dir);
		std::cout << "Done.\n";
	}
	return 0;
}
