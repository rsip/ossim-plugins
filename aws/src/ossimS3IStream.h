#ifndef ossimS3IStream_HEADER
#define ossimS3IStream_HEADER 1
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimFileInfoInterface.h>
#include "ossimS3StreamBuffer.h"

namespace ossim{

   class S3IStream : public ossim::istream
   {
   public:
      S3IStream():std::istream(&m_s3membuf)
      {}
      
      void open (const char* connectionString,  
                 const ossimKeywordlist& options,
                 
                 std::ios_base::openmode mode)
      {
         open(std::string(connectionString), options, mode);
      }
      void open (const std::string& connectionString, 
                 const ossimKeywordlist& options,
                 std::ios_base::openmode mode)
      {
         if(m_s3membuf.open(connectionString, options, mode))
         {
            clear();
         }
         else
         {
            setstate(std::ios::failbit);
         }
      }
      
      virtual ossim_int64 getFileSize() const
      {
         return (ossim_int64)m_s3membuf.getFileSize();
      }
      
      ossim_uint64 getBlockSize() const
      {
         return m_s3membuf.getBlockSize();
      }
      
   protected:
      S3StreamBuffer m_s3membuf;
   };
}

#endif
