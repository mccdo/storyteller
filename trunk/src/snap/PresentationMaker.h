#pragma once

#include <string>
#include <vector>
#include <map>

#include <Poco/Path.h>

#include "Exports.h"

/// @file PresentationMaker.h
/// @namespace snap
/// @class PresentationMaker makes an ODP-compliant presentation from a list
/// of image files. Add images to the presentation by calling AddImage(),
/// AddImages(), or AddDirectory(), then write out the presentation to disk
/// by calling MakePresentation(). Images are placed in the presentation in the
/// order they were given to AddImage or AddImages. If AddDirectory is used,
/// images are placed in natural sort order (ie. Image2.jpg is placed before
/// Image100.jpg, contrary to the usual alphanumeric sort algorithm).
namespace snap
{

class SNAP_EXPORT PresentationMaker
{
public:
    PresentationMaker();


    /// Add the image referenced by the path @c image
    void AddImage( const std::string& image );

    /// Add the images referred to by @c images. Each entry in the vector should
    /// be the relative or absolute path of the image file.
    void AddImages( const std::vector< std::string >& images );

    /// Add all images found in directory @directory. If @c recursive is true,
    /// recursively add all images in all subdirectories. Files are added in
    /// natural sort order, then, if recursive is true, subdriectories are
    /// entered in natural sort order.
    void AddDirectory( const std::string& directory, bool recursive = false );

    /// Returns a read-only reference to the list of images that will used to make
    /// the presentation. If you need to be able to directly alter the list,
    /// use GetImageListEditable().
    const std::vector<std::string>& GetImageList(){ return m_imageList; }

    /// Returns a reference to the list of images that will be used to
    /// make the presentation. This is provided so the list can be modified if
    /// necessary, for example to remove entries or change the order of entries,
    /// before writing out the presentation. If you only need the
    /// image list for display purposes (in a GUI, for example) use
    /// GetImageList() instead.
    std::vector<std::string>& GetImageListEditable(){ return m_imageList; }

    /// Make a presentation (odp) from all images added via AddImage, AddImages,
    /// and AddDirectory.
    void MakePresentation( const std::string& docname );

protected:
    virtual void BuildDirectoryStructure( const Poco::Path& path );
    virtual void ProcessImages( const Poco::Path& path );
    virtual void WriteMeta( const Poco::Path& path );
    virtual void WriteContent( const Poco::Path& path );
    virtual void WriteManifest( const Poco::Path& path );
    virtual void WriteUnchangedFiles( const Poco::Path& path );
    virtual void CreateArchive( const Poco::Path& filePath, const Poco::Path& snapTmp );

private:
    /// Delete directory referred to in path
    void RemoveDirectory( const Poco::Path &path );

    /// Compares file extension to internal list of image extensions known
    /// to work with vanilla OpenOffice/LibreOffice.
    bool FileIsImage( const std::string& extension );

    /// In-place conversion to lower case with locale awareness
    void LocaleAwareToLower( std::string& text );

    std::vector< std::string > m_extensionList;
    std::vector< std::string > m_imageList;
    std::string m_pageBlock;
    std::string m_manifestBlock;
    std::map< std::string, std::string > m_mimetypeMap;
};

} //namespace
