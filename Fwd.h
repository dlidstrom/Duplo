#ifndef _FWD_H_
#define _FWD_H_

#include <memory>

struct IFileType;
typedef std::shared_ptr<IFileType> IFileTypePtr;

struct ILineFilter;
typedef std::shared_ptr<ILineFilter> ILineFilterPtr;

class Options;
class SourceFile;
class SourceLine;

#endif
