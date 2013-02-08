/*
 * Copyright (C) 2012 BMW Car IT GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class LicenseHeaderChecker {

	private static List<File> allFilesToCheck = new ArrayList<File>();

	/**
	 * @param args
	 * @throws IOException
	 */
	public static void main(String[] args) throws IOException {
		System.out.println("License header checker");
		System.out.println("Usage: license regex (directory)");
		System.out.println("  license: filename that contains the license to check for");
		System.out.println("  regex: java regex which files to check (example: \".*\\.(h|cpp|c|hpp)\"");
		System.out.println("  directory: optionally the base directory to search through recursively (uses current dir otherwise))\n");
		if (args.length <= 2) {
			System.err.println("Wrong number of parameters (should be at least 2)");
		}
		String regex = args[1];
		File currentDirectory;
		if (args.length > 2) {
			currentDirectory = new File(args[2]);
		} else {
			currentDirectory = new File(System.getProperty("user.dir"));
		}
		File headerfile = new File(args[0]);
		String license = new Scanner(headerfile).useDelimiter("\\Z/").next();
		System.out.println("Checking for this content:\n" + license);
		System.out.println("-- End of content to check for.");
		System.out.println("Checking files matching regex: " + regex);

		collectAllFiles(currentDirectory, regex);
		boolean status = checkAllFiles(license);
		if (status){
			System.out.println("All checked files started with the license header");
		}else{
			System.err.println("ERROR: Found files without header!");
			System.exit(1);
		}
	}

	private static boolean checkAllFiles(String license)
			throws IOException {
		boolean result = true;
		for (File f : allFilesToCheck) {
			String fromFile = new Scanner(f).useDelimiter("\\Z/").next();
			if (!fromFile.startsWith(license)) {
				System.err.println("File " + f.getPath() + " does not start with given license header!");
				result = false;
			}else{
				System.out.println(f.getPath() + " checked.");
			}
		}
		return result;
	}

	public static void collectAllFiles(File current, String regex) {
		File[] files = current.listFiles();
		for (File file : files) {
			if (file.isDirectory()) {
					collectAllFiles(file, regex);
			} else {
				if (file.getName().matches(regex))
					allFilesToCheck.add(file);
			}
		}
	}

}
