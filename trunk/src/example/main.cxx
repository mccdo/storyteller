/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2012 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/
#include <iostream>

#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/AutoPtr.h>

#include <storyteller/PresentationMaker.h>

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;

// This application uses the Poco::Application class. If you're unfamiliar with
// it, this file may look a bit strange -- no int main( int argc, char** argv ),
// the whole thing is set up as a class, there are functions here that are
// not explicitly called anywhere in this file, etc. Don't let that bother you.
// Here's what's going on:
//
// Poco::Application contains the actual "main" entry point.
// The defineOptions method is called by Poco::Application to get a list of
// commandline parameters it should look for. It parses the commandline
// parameters and the "handleX" methods are called in response to the presence
// of these commandline parameters, setting up the state of the application.
// The "main" method (near the bottom of this file) is finally called, and is
// equivalent to the usual main() entry point, except that rather than being
// called first, it's called after the rest of this setup has taken place.
//
// Without the complexity of commandline parameter processing,
// this sample application could look something like this:
//
// int main( )
// {
//     storyteller::PresentationMaker pm;
//     pm.AddDirectory( "SomeTestDirectoryWithImages" );
//     pm.MakePresentation();
//     return 0;
// }

class Storyteller: public Application
{
public:
    Storyteller():
        m_noOp( true ),
        m_helpRequested( false ),
        m_recursive( false ),
        m_showVersion( false )
    {
    }

protected:
    void defineOptions(OptionSet& options)
    {
        Application::defineOptions(options);

        Option recursive("recursive", "r",
                         "Recursively add subdirectories. Has an effect only on the *first* directory parameter following it. This option can be repeated before each directory.",
                         false );
        recursive.repeatable(true)
                        .callback( OptionCallback<Storyteller>(this,
                                                 &Storyteller::handleRecursive) );

        Option outputFile("output", "o",
                          "Presentation output file name", false,
                          "OutputFile", true );
        outputFile.repeatable(false)
                .callback( OptionCallback<Storyteller>(this,
                                                &Storyteller::handleOutputName) );

        Option directory("directory", "d",
                                     "Add all image files in directory. Images are added in the order they are found on disk; no sorting is done.", false,
                                     "Directory", true );
        directory.repeatable( true )
                .callback( OptionCallback<Storyteller>(this,
                                                &Storyteller::handleDirectoryName) );

        Option image("image", "i", "Add a single image of type .jpg, .tiff, .bmp, .gif, .png, .pcx, or .xpm", false, "Image",
                                 true );
        image.repeatable( true )
                .callback( OptionCallback<Storyteller>(this,
                                                &Storyteller::handleImageName) );

        Option help("help", "h", "Display this help message and exit");
        help.required(false).repeatable(false)
            .callback(OptionCallback<Storyteller>(this, &Storyteller::handleHelp));

        Option version("version", "v", "Display version and exit");
        version.required(false).repeatable(false)
            .callback(OptionCallback<Storyteller>(this, &Storyteller::handleVersion));

        options.addOption( help );
        options.addOption( directory );
        options.addOption( image );
        options.addOption( outputFile );
        options.addOption( recursive );
        options.addOption( version );
    }

    void handleHelp( const std::string&, const std::string& )
    {
        m_helpRequested = true;
        displayHelp();
        stopOptionsProcessing();
    }

    void handleRecursive( const std::string&, const std::string& )
    {
        m_recursive = true;
    }

    void handleDirectoryName( const std::string&, const std::string& value )
    {
        m_presentationMaker.AddDirectory( value, m_recursive );
        m_recursive = false;
    }

    void handleImageName( const std::string&, const std::string& value )
    {
        m_presentationMaker.AddImage( value );
    }

    void handleOutputName( const std::string&, const std::string& value )
    {
        m_outputFileName = value;
        m_noOp = false;
    }

    void handleVersion( const std::string&, const std::string& )
    {
        m_noOp = false;
       m_showVersion = true;
    }

    void displayHelp()
    {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("Builds an ODP-compliant presentation from a group of image files.");
        helpFormatter.setFooter( "Example use: storyteller -dDirectoryA -r -dDirectoryB -iImage1 -iImage2 -oMyPresentation.odp\n\nThis example would create a presentation named \"MyPresentation.odp\" from all images found in DirectoryA, all images found in DirectoryB and all of its subdirectories (and their subdirectories, etc.), Image1, and Image2.\n" );
        helpFormatter.format(std::cout);
    }

    int main(const std::vector<std::string>& )
    {
        if( m_noOp )
        {
            displayHelp();
            return Application::EXIT_OK;
        }

        if( m_showVersion )
        {
            std::cout << "storyteller version 0.1" << std::endl;
            return Application::EXIT_OK;
        }

        if (!m_helpRequested)
        {
            m_presentationMaker.MakePresentation( m_outputFileName );
        }
        return Application::EXIT_OK;
    }

private:
    bool m_noOp;
    bool m_helpRequested;
    bool m_recursive;
    bool m_showVersion;
    std::string m_outputFileName;
    storyteller::PresentationMaker m_presentationMaker;
};


POCO_APP_MAIN(Storyteller)
