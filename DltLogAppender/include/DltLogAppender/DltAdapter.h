/*
 * Copyright (C) 2015 BMW Car IT GmbH
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

#ifndef CAPU_DLTADAPTER_H
#define CAPU_DLTADAPTER_H

#include "capu/container/String.h"
#include "capu/container/Vector.h"
#include "capu/container/HashSet.h"

#include "capu/util/LogContext.h"
#include "capu/util/Logger.h"

namespace capu
{
    enum DltError
    {
        DLT_ERROR_NO_ERROR = 0x0,
        DLT_ERROR_CONTEXT_INVALID,
        DLT_ERROR_MISSING_DLT_CONTEXT,
        DLT_ERROR_INIT,
        DLT_ERROR_DEINIT,
        DLT_ERROR_USER_LOG,
        DLT_ERROR_REGISTER_APPLICATION_FAILED,
        DLT_ERROR_REGISTER_CONTEXT_FAILED,
        DLT_ERROR_UNREGISTER_CONTEXT_FAILED,
        DLT_ERROR_UNREGISTER_APPLICATION_FAILED,
        DLT_ERROR_INJECTION_CALLBACK_FAILURE,
        DLT_ERROR_FILETRANSFER_FAILURE
    };

    class DltContextPointer
    {
    public:
        DltContextPointer(){}

        DltContextPointer(void* ptr)
        {
            setPtr(ptr);
        }
        ~DltContextPointer(){};

        void setPtr(void* ptr)
        {
            m_dltContextPtr = ptr;
        }

        void* getPtr()
        {
            return m_dltContextPtr;
        }
    private:
        void* m_dltContextPtr;
    };

    /**
     * Singleton for DltAdapter
     */
    class DltAdapter
    {
    public:
        /**
         * Get the singleton instance
         * @returns the DltAdapter singleton
         */
        static DltAdapter* getDltAdapter();

        /**
         * Send log message to dlt
         * @param msg a string containing the log message
         */
        void logMessage(const capu::LogMessage& msg);

        /**
         * Register new context, if context already exists no new context is created
         * @param ctx capu log context to use
         * @returns void ptr to the newly created dlt context
         */
        void* registerContext(capu::LogContext* ctx, const capu::String& id);

        /**
         * Unregister existing context. Method checks if passed context exists before unregistering
         * @param ctx the context to unregister
         */
        void unregisterContext(capu::LogContext* ctx);

        /**
         * Register new application. Every application has one unique name
         * @param id id string for application
         * @param desc description text with more details than the name
         */
        void registerApplication(const String& id, const String& desc);

        /**
         * Unregister Application
         */
        void unregisterApplication();

        /**
         * Register injection callback
         * @param ctx the dlt context to use
         * @param sid the service id of the callback
         * @param dltInjectionCallback the function to be called trough the callback
         */
        void registerInjectionCallback(capu::LogContext* ctx,uint32_t sid,int (*dltInjectionCallback)(uint32_t service_id, void *data, uint32_t length));

        /**
         * Transmit a file via DLT
         * @param uri the path to the file
         * @param delFile delete file afterwards
         * @timeout timeout for file transmission
         */
        void transmitFile(const String& uri, int delFile, int timeout);

        /**
         * Returns the application name
         * @returns name of the application
         */
        const String& getApplicationName()
        {
            return m_appName;
        }

        /**
         * Returns the description of the application
         * @return description of the application
         */
        const String& getApplicationDescription()
        {
            return m_appDesc;
        }

        /**
         * Return the state if dlt was found at runtime
         * @return if dlt was successfully initialized
         */
        bool isDltInitialized()
        {
            return m_dltInitialized;
        }

        /**
         * Get the error status of the DltAdapter
         * DLT_ERROR_NO_ERROR indicates that no error occured
         * @returns enum with error code and clears the current error
         */
        DltError getDltStatus()
        {
            DltError cur = m_dltError;
            m_dltError = DLT_ERROR_NO_ERROR;
            return cur;
        }

    private:

        /**
         * Private constructor
         */
        DltAdapter();

        /**
         * Private destructor
         */
        ~DltAdapter();

        /**
         * Get the dlt context ptr for a given LogContext
         * @param context the ramses LogContext
         * @returns void DltContextPointer for given LogContext
         */
        DltContextPointer getDltContext(const LogContext* context);

    private:

        /**
         * Name of the application,4 char string
         */
        String m_appName;

        /**
         * More detailed description of the application
         */
        String m_appDesc;

        /**
         * Context for filetransfers
         */
        static void* s_fileContext;

        /**
         * Bool to check if DLT was initialized correctly
         * if dlt libs are not there applications will not start due to linker failure
         * if dlt libs are there but dlt-daemon is not running dlt_init return negative
         * values, in which case this variable stays false
         */
        bool m_dltInitialized;

        /**
         * Internal flag to track errors
         */
        DltError m_dltError;

        /**
         * Vector of all created contexts.
         */
        Vector<void*> m_dltContextList;
    };
}
#endif // RAMSES_DLTADAPTER_H
