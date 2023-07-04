//
// Copyright 2016 -- 2018 PMSF IT Consulting Pierre R. Mai
// Copyright 2023 BMW AG
// SPDX-License-Identifier: MPL-2.0
// The MPL-2.0 is only an example here. You can choose any other open source license accepted by OpenMSL, or any other license if this template is used elsewhere.
//

#pragma once

#include "osi_sensordata.pb.h"
struct ObjectInfo
{
    int id;
    int age;
    int movement_state;  // 0:stationary, 1:moving, 2:stopped
};
class OSMPCameraSensor
{
  public:
    void Init(double nominal_range_in);
    osi3::SensorData Step(osi3::SensorView current_in, double time);

    static void RotatePoint(double x, double y, double z, double yaw, double pitch, double roll, double& rx, double& ry, double& rz);

  private:
    double nominal_range_;

    static void TransposeRotationMatrix(double matrix_in[3][3], double matrix_trans[3][3]);

    /* Private File-based Logging just for Debugging */
#ifdef PRIVATE_LOG_PATH
    static ofstream private_log_file;
#endif

    static void FmiVerboseLogGlobal(const char* format, ...)
    {
#ifdef VERBOSE_FMI_LOGGING
#ifdef PRIVATE_LOG_PATH
        va_list ap;
        va_start(ap, format);
        char buffer[1024];
        if (!private_log_file.is_open())
            private_log_file.open(PRIVATE_LOG_PATH, ios::out | ios::app);
        if (private_log_file.is_open())
        {
#ifdef _WIN32
            vsnprintf_s(buffer, 1024, format, ap);
#else
            vsnprintf(buffer, 1024, format, ap);
#endif
            private_log_file << "OSMPCameraSensor"
                             << "::Global:FMI: " << buffer << endl;
            private_log_file.flush();
        }
#endif
#endif
    }

    void InternalLog(const char* category, const char* format, va_list arg)
    {
#if defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING)
        char buffer[1024];
#ifdef _WIN32
        vsnprintf_s(buffer, 1024, format, arg);
#else
        vsnprintf(buffer, 1024, format, arg);
#endif
#ifdef PRIVATE_LOG_PATH
        if (!private_log_file.is_open())
            private_log_file.open(PRIVATE_LOG_PATH, ios::out | ios::app);
        if (private_log_file.is_open())
        {
            private_log_file << "OSMPCameraSensor"
                             << "::" << instanceName << "<" << ((void*)this) << ">:" << category << ": " << buffer << endl;
            private_log_file.flush();
        }
#endif
#ifdef PUBLIC_LOGGING
        if (loggingOn && loggingCategories.count(category))
            functions.logger(functions.componentEnvironment, instanceName.c_str(), fmi2OK, category, buffer);
#endif
#endif
    }

    void FmiVerboseLog(const char* format, ...)
    {
#if defined(VERBOSE_FMI_LOGGING) && (defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING))
        va_list ap;
        va_start(ap, format);
        internal_log("FMI", format, ap);
        va_end(ap);
#endif
    }

    /* Normal Logging */
    void NormalLog(const char* category, const char* format, ...)
    {
#if defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING)
        va_list ap;
        va_start(ap, format);
        internal_log(category, format, ap);
        va_end(ap);
#endif
    }
};