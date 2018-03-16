//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************
#ifndef AtpTileSource_HEADER
#define AtpTileSource_HEADER

#include "AtpConfig.h"
#include "AutoTiePoint.h"
#include "AtpGenerator.h"
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <memory>
#include <vector>

namespace ATP
{

/**
 * Base class for tile sources performing auto tie point extraction. Implemented as a combiner that
 * establishes the overlap between input sources, and computes tie-points in the getTile() call.
 */
class OSSIMDLLEXPORT AtpTileSource : public ossimImageCombiner
{
public:
   AtpTileSource();
   AtpTileSource(ossimConnectableObject::ConnectableObjectList& inputSources);
   AtpTileSource(AtpGenerator* generator);

   virtual ~AtpTileSource() {}

   virtual void initialize();

   /**
    * Derived classes implement their particular tiepoint matching algorithms for the requested
    * tile. It is not clear what the output tile needs to be, since the product of this filter
    * is the list of tiepoints (available through the getTiePoints() method). Recommend simply
    * passing through the corresponding REF image tile.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin, ossim_uint32 rLevel=0) = 0;

   /** This list resets with every getTile() call , so make sure tiepoints from previous getTile()
    *  calls are being accumulated */
   AtpList& getTiePoints() { return m_tiePoints; }

   virtual ossimScalarType getOutputScalarType() const { return OSSIM_DOUBLE; }
   virtual ossim_uint32 getNumberOfOutputBands() const { return 2; }

   const double& getMaxResidualMagnitude() const { return m_maxResidualMagnitude; }

   ossimRefPtr<ossimImageViewProjectionTransform>& getRefIVT() { return m_refIVT; }
   ossimRefPtr<ossimImageViewProjectionTransform>& getCmpIVT() { return m_cmpIVT; }

   /** The view geometry is used to initialize the view side of the two IVTs. */
   void setViewGeom(ossimImageGeometry* geom);

   std::string getRefImageID();
   std::string getCmpImageID();
   std::string getRefFilename();
   std::string getCmpFilename();

protected:
   virtual void allocate();
   ossimImageHandler* getImageHandler(ossimRefPtr<ossimImageSource>& chain);
   void filterPoints();

   //! Removes inconsistent residual peaks
   void removeBadMatches();

   //! Caps the max number of TPs given the list, which is the list of filtered TPs for the tile.
   void pruneList();

   std::shared_ptr<AtpGenerator> m_generator;
   ossimRefPtr<ossimImageSource> m_refChain;
   ossimRefPtr<ossimImageSource> m_cmpChain;
   ossimRefPtr<ossimImageViewProjectionTransform> m_refIVT;
   ossimRefPtr<ossimImageViewProjectionTransform> m_cmpIVT;
   AtpList m_tiePoints;
   ossimRefPtr<ossimImageData> m_tile; // Used only for raster mode (rare)
   double m_maxResidualMagnitude;
};
}
#endif /* #ifndef AtpTileSource_HEADER */
