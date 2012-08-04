#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <locale>
#include <algorithm>
#include <iomanip>

#include <Poco/Zip/Compress.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Environment.h>
#include <Poco/RegularExpression.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>
#include <Poco/String.h>

#include "PresentationMaker.h"
#include "OdpXmlFiles.h"
#include "alphanum.h"

namespace snap
{

PresentationMaker::PresentationMaker()
{
    // no doubt others should be in this list; need to find a proper list
    // of the image formats that are supported in vanilla installs of
    // openoffice or libreoffice

    // Allowed image file extensions, lowercase versions only to keep things
    // simple
    m_extensionList.push_back("jpg");
    m_extensionList.push_back("jpeg");
    m_extensionList.push_back("jpe");
    m_extensionList.push_back("bmp");
    m_extensionList.push_back("bitmap");
    m_extensionList.push_back("xpm");
    m_extensionList.push_back("gif");
    m_extensionList.push_back("tif");
    m_extensionList.push_back("tiff");
    m_extensionList.push_back("png");
    m_extensionList.push_back("pcx");

    // Key is extension (as above); value is the mime-type
    m_mimetypeMap["jpg"] = "image/jpeg";
    m_mimetypeMap["jpeg"] = "image/jpeg";
    m_mimetypeMap["jpe"] = "image/jpeg";
    m_mimetypeMap["bmp"] = "image/bmp";
    m_mimetypeMap["bitmap"] = "image/bmp";
    m_mimetypeMap["xpm"] = "image/xpm";
    m_mimetypeMap["gif"] = "image/gif";
    m_mimetypeMap["tif"] = "image/tif";
    m_mimetypeMap["tiff"] = "image/tif";
    m_mimetypeMap["png"] = "image/png";
    m_mimetypeMap["pcx"] = "image/pcx";

    // TODO: Replace hard-coded file extensions and mime-types with reading
    // this info from a text file so that users can more easily modify the types
    // to their needs.
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::AddImage( const std::string& image )
{
    m_imageList.push_back( image );
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::AddImages( const std::vector< std::string >& images )
{
    std::vector< std::string >::const_iterator itr = images.begin();
    while( itr != images.end() )
    {
        m_imageList.push_back( *itr );
        ++itr;
    }
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::AddDirectory( const std::string& directory,
                                      bool recursive )
{
    std::cout << "Searching for images in " << directory << "..."
              << std::flush;
    std::vector<std::string> files;
    std::vector<std::string> dirs;
    Poco::Path path( directory );
    Poco::DirectoryIterator itr( path );
    Poco::DirectoryIterator end;
    while( itr != end )
    {
        if( itr->isFile() )
        {
            if( FileIsImage( itr.path().getExtension() ) )
            {
                files.push_back( itr.path().toString() );
            }
        }
        else if( itr->isDirectory() )
        {
            dirs.push_back( itr.path().toString() );
        }
        ++itr;
    }

    // Natural sort
    std::sort( files.begin(), files.end(), doj::alphanum_less<std::string>() );
    std::sort( dirs.begin(), dirs.end(), doj::alphanum_less<std::string>() );

    AddImages( files );

    std::cout << "done." << std::endl << std::flush;

    if( recursive )
    {
        std::vector<std::string>::const_iterator dir_itr = dirs.begin();
        while( dir_itr != dirs.end() )
        {
            AddDirectory( *dir_itr, true );
            ++dir_itr;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
bool PresentationMaker::FileIsImage( const std::string& extension )
{
    std::string lowerCaseExtension( extension );
    LocaleAwareToLower( lowerCaseExtension );

    std::vector< std::string >::const_iterator itr = m_extensionList.begin();
    while( itr != m_extensionList.end() )
    {
        if( lowerCaseExtension == *itr )
        {
            return true;
        }
        ++itr;
    }

    return false;
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::MakePresentation( const std::string& docname )
{
    Poco::Path fullPath( docname );

    // Temp directory snapTmp = fullPath_without_filename/snap_tmp
    Poco::Path snapTmp( fullPath );
    snapTmp.setFileName( "" );
    snapTmp.pushDirectory( "snap_tmp" );

    BuildDirectoryStructure( snapTmp );

    ProcessImages( snapTmp );

    WriteManifest( snapTmp );

    WriteContent( snapTmp );

    WriteUnchangedFiles( snapTmp );

    WriteMeta( snapTmp );

    CreateArchive( fullPath, snapTmp );

    RemoveDirectory( snapTmp );
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::RemoveDirectory( const Poco::Path& path )
{
    Poco::File dir( path );
    if( dir.exists() )
    {
        // "rm -r dir"
        dir.remove( true );
    }
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::BuildDirectoryStructure( const Poco::Path& path )
{
    RemoveDirectory( path );

    Poco::File tmpFile( path );
    // "mkdir path"
    tmpFile.createDirectory();

    // Subdirs are Pictures, META-INF, Thumbnails
    std::string base( path.toString(Poco::Path::PATH_UNIX) );
    Poco::File f( base + "/Pictures" );
    f.createDirectory();
    f = base + "/META-INF";
    f.createDirectory();
    f = base + "/Thumbnails";
    f.createDirectory();
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::ProcessImages( const Poco::Path& path )
{
    std::cout << "Processing " << m_imageList.size() << " images..."
              << std::flush;
    Poco::Path destPath( path );
    destPath.pushDirectory( "Pictures" );

    for( size_t index = 0; index < m_imageList.size(); ++index )
    {
        // Form new name as 32-character-wide number field + extension.
        Poco::Path imagePath( m_imageList.at( index ) );
        std::stringstream nameSS;
        nameSS << std::setw(32) << std::setfill('0') << ( index + 1 ) << ".";
        std::string newFileName( nameSS.str() );
        newFileName.append( imagePath.getExtension() );

        // Copy image from original path to Pictures subdir with new name.
        Poco::File imageFile( imagePath );
        destPath.setFileName( newFileName );
        imageFile.copyTo( destPath.toString() );

        // Fill out a page entry
        std::stringstream pageNum;
        pageNum << index + 1;
        std::string pageData( pageEntry );
        {
            Poco::RegularExpression regex( "##PAGE_NUMBER##" );
            regex.subst( pageData, pageNum.str(),
                         Poco::RegularExpression::RE_GLOBAL );
        }
        {
            Poco::RegularExpression regex( "##IMAGE_NAME##" );
            regex.subst( pageData, newFileName,
                         Poco::RegularExpression::RE_GLOBAL );
        }
        m_pageBlock.append( pageData );

        // Fill out a manifest entry
        std::string lowerCaseExtension( imagePath.getExtension() );
        LocaleAwareToLower( lowerCaseExtension );
        std::string mimeType = m_mimetypeMap[ lowerCaseExtension ];
        std::string manifestData( manifestEntry );
        {
            Poco::RegularExpression regex( "##IMAGE_TYPE##" );
            regex.subst( manifestData, mimeType,
                         Poco::RegularExpression::RE_GLOBAL );
        }
        {
            Poco::RegularExpression regex( "##IMAGE_NAME##" );
            regex.subst( manifestData, newFileName,
                         Poco::RegularExpression::RE_GLOBAL );
        }
        m_manifestBlock.append( manifestData );
    }
    std::cout << "done." << std::endl << std::flush;
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::LocaleAwareToLower( std::string& text )
{
    // As suggested by Johannes Schaub at
    // http://stackoverflow.com/questions/313970/stl-string-to-lower-case
    //std::transform(text.begin(), text.end(), text.begin(),
    //    std::bind2nd(std::ptr_fun(&std::tolower<char>), std::locale("")));
    Poco::toLowerInPlace( text );
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::WriteMeta( const Poco::Path& path )
{
    Poco::Path localPath( path );
    // Get username, date, time, and calculate objects to write into meta
    std::string username = Poco::Environment::get( "USER", "unknown" );
    if( username == "unknown" )
    {
        username = Poco::Environment::get( "%USERNAME%", "unknown" );
    }
    Poco::LocalDateTime ct;
    std::stringstream dateTimeString;
    dateTimeString << ct.year() << "-"
                   << std::setw(2) << std::setfill('0') << ct.month() << "-"
                   << std::setw(2) << std::setfill('0') << ct.day() << "T"
                   << std::setw(2) << std::setfill('0') << ct.hour() << ":"
                   << std::setw(2) << std::setfill('0') << ct.minute() << ":"
                   << std::setw(2) << std::setfill('0') << ct.second();

    // object count starts at 21 for a file with no pictures. Each slide with a
    // picture increments the count by 3.
    std::stringstream objectCount;
    objectCount << 21 + 3 * m_imageList.size();

    std::string metaData( meta );
    {
        Poco::RegularExpression regex( "##DATE_TIME##" );
        regex.subst( metaData, dateTimeString.str(),
                     Poco::RegularExpression::RE_GLOBAL );
    }
    {
        Poco::RegularExpression regex( "##USERNAME##" );
        regex.subst( metaData, username, Poco::RegularExpression::RE_GLOBAL );
    }
    {
        Poco::RegularExpression regex( "##OBJECT_COUNT##" );
        regex.subst( metaData, objectCount.str(),
                     Poco::RegularExpression::RE_GLOBAL );
    }
    {
        Poco::RegularExpression regex( "##GENERATOR_NAME##" );
        regex.subst( metaData, "snap 0.1", Poco::RegularExpression::RE_GLOBAL );
    }

    localPath.setFileName( "meta.xml" );
    std::ofstream outFile( localPath.toString(Poco::Path::PATH_NATIVE).c_str() );
    outFile << metaData;
    outFile.close();
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::WriteContent( const Poco::Path& path )
{
    std::string contentData( content );
    Poco::RegularExpression regex( "##PAGE_LIST##" );
    regex.subst( contentData, m_pageBlock,
                 Poco::RegularExpression::RE_GLOBAL );

    Poco::Path localPath( path );
    localPath.setFileName( "content.xml" );
    std::ofstream outFile( localPath.toString(Poco::Path::PATH_NATIVE).c_str() );
    outFile << contentData;
    outFile.close();
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::WriteManifest( const Poco::Path& path )
{
    std::string manifestData( manifest );
    Poco::RegularExpression regex( "##IMAGE_LIST##" );
    regex.subst( manifestData, m_manifestBlock,
                 Poco::RegularExpression::RE_GLOBAL );

    Poco::Path localPath( path );
    localPath.pushDirectory("META-INF");
    localPath.setFileName( "manifest.xml" );
    std::ofstream outFile( localPath.toString(Poco::Path::PATH_NATIVE).c_str() );
    outFile << manifestData;
    outFile.close();
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::WriteUnchangedFiles( const Poco::Path& path )
{
    Poco::Path localPath( path );

    localPath.setFileName( "settings.xml" );
    std::ofstream outFile( localPath.toString(Poco::Path::PATH_NATIVE).c_str() );
    outFile << settings;
    outFile.close();

    localPath.setFileName( "mimetype" );
    outFile.open( localPath.toString(Poco::Path::PATH_NATIVE).c_str() );
    outFile << mimetype;
    outFile.close();

    localPath.setFileName( "styles.xml" );
    outFile.open( localPath.toString(Poco::Path::PATH_NATIVE).c_str() );
    outFile << styles;
    outFile.close();
}
////////////////////////////////////////////////////////////////////////////////
void PresentationMaker::CreateArchive( const Poco::Path& filePath, const Poco::Path& snapTmp )
{
    std::cout << "Compressing archive..." << std::flush;
    std::ofstream odp( filePath.toString().c_str() );
    Poco::Zip::Compress zipper( odp, true );
    zipper.addRecursive( snapTmp );
    zipper.close();
    std::cout << "done." << std::endl << std::flush;
}
////////////////////////////////////////////////////////////////////////////////

}//namespace
