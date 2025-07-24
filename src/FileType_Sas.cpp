#include "FileType_Sas.h"
#include "SasStyleCommentsFilter.h"
#include "Utils.h"
#include <unordered_set>

static const std::unordered_set<std::string> IGNORE_LINES = {
    "%let datetime_start = %sysfunc(datetime()) ;",
    "data _null_;",
    "startdate = \"%sysfunc(date(),worddate.)\";",
    "starttime = \"%sysfunc(time(),time.)\";",
    "put \"===================================================================================\";",
    "put \"  starting job information:\" /;",
    "put \"  job name: input file contents\" /;",
    "put \"  start date: \" startdate ;",
    "put \"  start time: \" starttime ;",
    "put \"===================================================================================\";",
    "put \"  start date: \" startdate;",
    "put \"  start time: \" starttime;",
    "enddate = \"%sysfunc(date(),worddate.)\";",
    "endtime = \"%sysfunc(time(),time.)\";",
    "processing_time = put(%sysevalf(%sysfunc(datetime())-&datetime_start.),time8.0) ;",
    "put \"  ending job information:\" /;",
    "put \"  job name: create confidence intervals dx payment\" /;",
    "put \"  end date: \" enddate ;",
    "put \"  end time: \" endtime ;",
    "put \"  processing time: \" processing_time ;",
    "put \"  end date: \" enddate;",
    "put \"  end time: \" endtime;",
    "run;",
};

FileType_Sas::FileType_Sas(bool ignorePrepStuff, unsigned minChars)
    : FileTypeBase(ignorePrepStuff, minChars) {
}

ILineFilterPtr FileType_Sas::CreateLineFilter() const {
    return std::make_shared<SasStyleCommentsLineFilter>();
}

std::string FileType_Sas::GetCleanLine(const std::string& line) const {
    return CstyleUtils::RemoveSingleLineComments(line);
}

bool FileType_Sas::IsPreprocessorDirective(const std::string& line) const {
    return IGNORE_LINES.find(line) != IGNORE_LINES.end();
}
