//
//  diff.cpp
//  pvc
//
//  Created by Jack on 6/16/18.
//  Copyright © 2018 JackMacWindows. All rights reserved.
//

#include "diff.hpp"
#include "pull.hpp"

std::vector<patch> diffStrings(std::string text1, std::string text2) {
	std::vector<patch> retval;
	if (text1 == "") {
		patch p;
		p.lineno = 1;
		p.content = text2;
		p.type = PATCH_NEW_FILE;
		retval.push_back(p);
		return retval;
	}
	int lineAdvance = 0;
	std::stringstream ss1(text1.c_str());
	std::stringstream ss2(text2.c_str());
	strvec lines1, lines2;
	std::string temp;
	while (std::getline(ss1, temp, '\n')) {
		lines1.push_back(temp);
	}
	while (std::getline(ss2, temp, '\n')) {
		lines2.push_back(temp);
	}
	for (int i = 0; i < lines1.size() || i + lineAdvance < lines2.size(); i++) {
		int newLineAdvance = lineAdvance;
		if (i < lines1.size() && std::find(lines2.begin() + i + lineAdvance, lines2.end(), lines1[i]) == lines2.end()/* && lines1[i] != lines2[lines2.size()-1]*/) {
			patch p;
			p.lineno = i + lineAdvance + 1;
			p.type = PATCH_DEL_LINE;
			p.content = lines1[i];
			retval.push_back(p);
			newLineAdvance--;
		}
		if (i + lineAdvance < lines2.size() && std::find(lines1.begin() + i, lines1.end(), lines2[i+lineAdvance]) == lines1.end()/* && lines2[i+lineAdvance] != lines1[lines1.size()-1]*/) {
			patch p;
			p.lineno = i + lineAdvance + 1;
			p.type = PATCH_ADD_LINE;
			p.content = lines2[i+lineAdvance];
			retval.push_back(p);
			newLineAdvance++;
		}
		i -= (newLineAdvance != lineAdvance);
		lineAdvance = newLineAdvance;
	}
	return retval;
}

std::vector<patch> diffFiles(std::string file1, std::string file2) {
	std::vector<patch> retval;
	if (!fileExists(file1)) {
		patch p;
		p.lineno = 1;
		p.content = convertFileData(readFile(file2));
		p.type = PATCH_NEW_FILE;
		p.filename = file2;
		retval.push_back(p);
		return retval;
	}
	if (!fileExists(file2)) {
		patch p;
		p.lineno = 1;
		p.content = "";
		p.type = PATCH_DEL_FILE;
		p.filename = file1;
		retval.push_back(p);
		return retval;
	}
	retval = diffStrings(convertFileData(readFile(file1)), convertFileData(readFile(file2)));
	for (std::vector<patch>::iterator it = retval.begin(); it != retval.end(); it++) it->filename = file1;
	return retval;
}

std::vector<patch> diffDirs(std::string dir1, std::string dir2) {
	std::vector<patch> retval;
	strvec files = listDir(dir1);
	strvec files2 = listDir(dir2);
	for (std::string f : files2) if (std::find(files.begin(), files.end(), f) == files.end()) files.push_back(f);
	for (std::string f : files) {
		if (isDirectory(dir2 + "/" + f) && !fileExists(dir1 + "/" + f)) {
			patch p;
			p.lineno = 1;
			p.type = PATCH_NEW_DIR;
			p.content = "";
			p.filename = f;
			retval.push_back(p);
		} else {
			if (!isDirectory(dir1 + "/" + f) && !isDirectory(dir2 + "/" + f)) {
				std::vector<patch> v = diffFiles(dir1 + "/" + f, dir2 + "/" + f);
				for (std::vector<patch>::iterator it = retval.begin(); it != retval.end(); it++) {
					if (it->filename.find(dir1) != std::string::npos)
						it->filename.replace(it->filename.begin() + it->filename.find(dir1),\
											 it->filename.begin() + it->filename.find(dir1) + dir1.size() + 1,\
											 "");
					if (it->filename.find(dir2) != std::string::npos)
						it->filename.replace(it->filename.begin() + it->filename.find(dir2),\
											 it->filename.begin() + it->filename.find(dir2) + dir2.size() + 1,\
											 "");
				}
				retval.insert(retval.end(), v.begin(), v.end());
			} else log("A directory (" + f + ") was checked while diffing.", MESSAGE_LOG);
		}
	}
	return retval;
}

std::vector<patch> diffCommits(commits state, std::string dir) {
	mkdir(std::string(std::string(TEMP_DIR) + "/.pvcCommitDiffTemp").c_str(), 0777);
	reconstructDir(state, std::string(std::string(TEMP_DIR) + "/.pvcCommitDiffTemp"));
	std::vector<patch> retval = diffDirs(std::string(std::string(TEMP_DIR) + "/.pvcCommitDiffTemp"), dir);
	strvec files = listDir(std::string(std::string(TEMP_DIR) + "/.pvcCommitDiffTemp"));
	for (std::string f : files) {
		if (isDirectory(f)) rmdir(f.c_str());
		else remove(f.c_str());
	}
	rmdir(std::string(std::string(TEMP_DIR) + "/.pvcCommitDiffTemp").c_str());
	return retval;
}
