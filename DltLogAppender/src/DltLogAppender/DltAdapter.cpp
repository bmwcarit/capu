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

#include "DltLogAppender/DltAdapter.h"

#include <dlt/dlt.h>
extern "C"
{
#include <dlt/dlt_filetransfer.h>
}

#define DLT_CHECK_ERROR(ERRCODE)         \
    if(status < 0)                       \
    {                                    \
        m_dltError=ERRCODE;              \
    }                                    \

namespace capu
{
    DltAdapter* DltAdapter::s_singleton = 0;

    void* DltAdapter::s_fileContext = 0;

    DltAdapter::DltAdapter()
    : m_appName("")
    , m_appDesc("")
    , m_dltInitialized(false)
    , m_dltError(DLT_ERROR_NO_ERROR)
    {
        int32_t status = 0;
        dlt_init();
        DLT_CHECK_ERROR(DLT_ERROR_INIT)
        if(status>=0)
        {
            if(dlt_set_log_mode(DLT_USER_MODE_BOTH)>=0)
            {
                m_dltInitialized=true;
            }
            else
            {
                printf("No DLT daemon connected, DLT functionality is disabled\n");
            }
        }
    }

    DltAdapter::~DltAdapter()
    {
        int32_t status = 0;

        unregisterApplication();

        status = dlt_free();
        DLT_CHECK_ERROR(DLT_ERROR_DEINIT)

        delete s_singleton;
        s_singleton = 0;

        m_dltInitialized = false;
    }

    DltAdapter* DltAdapter::getDltAdapter()
    {
        if(!s_singleton)
        {
            s_singleton = new DltAdapter();
        }
        return s_singleton;
    }

    void DltAdapter::logMessage(const capu::LogMessage& msg)
    {
        int32_t status = 0;

        if(!m_dltInitialized)
        {
            return;
        }
        if(&(msg.getContext()) == (void*)0)
        {
            m_dltError=DLT_ERROR_CONTEXT_INVALID;
            return;
        }

        DltContext* dltContext = static_cast<DltContext*>(getDltContext(&(msg.getContext())).getPtr());
        if(dltContext == (void*)0)
        {
            m_dltError=DLT_ERROR_MISSING_DLT_CONTEXT;
            return;
        }

        //DLT_LOG_OFF indicates that message is not logged
        //DLT_LOG_DEFAULT is unused here
        DltLogLevelType ll=DLT_LOG_OFF;
        switch(msg.getLogLevel())
        {
            case LL_TRACE:
            case LL_ALL:
                ll = DLT_LOG_VERBOSE;
                break;
            case LL_DEBUG:
                ll = DLT_LOG_DEBUG;
                break;
            case LL_INFO:
                ll = DLT_LOG_INFO;
                break;
            case LL_WARN:
                ll = DLT_LOG_WARN;
                break;
            case LL_ERROR:
                ll = DLT_LOG_ERROR;
                break;
            case LL_FATAL:
                ll = DLT_LOG_FATAL;
                break;
            default:
                //LL_OFF remains -> DLT_LOG_OFF is already set above
                break;
        }

        if(ll == DLT_LOG_OFF)
        {
            return; // no need to continue
        }

        if(dltContext)
        {
#ifdef DLT_CONTEXT_USE_LOG_LEVEL_PTR
            void* log_level_ptr = dltContext->log_level_ptr;
            if(log_level_ptr)
            {
                if(ll<=(uint8_t)*(dltContext->log_level_ptr) )
                {
#endif
                    DltContextData ctxData;
                    status = dlt_user_log_write_start(dltContext,&ctxData,ll);
                    if (status >= 0)
                    {
                        status = dlt_user_log_write_string(&ctxData,msg.getLogMessage());
                        status = dlt_user_log_write_finish(&ctxData);
                        DLT_CHECK_ERROR(DLT_ERROR_USER_LOG)
                    }
                    else
                    {
                        DLT_CHECK_ERROR(DLT_ERROR_USER_LOG)
                    }
#if DLT_CONTEXT_USE_LOG_LEVEL_PTR
                }
            }
#endif
        }
    }

    void DltAdapter::registerApplication(const String& id,const String& description)
    {
        int32_t status = 0;

        if(!m_dltInitialized)
        {
            return;
        }
        if(m_appName == id)
        {
            return;
        }
        if(m_appName.getLength()>0)
        {
            unregisterApplication();
        }

        status = dlt_register_app(id.c_str(),description.c_str());
        DLT_CHECK_ERROR(DLT_ERROR_REGISTER_APPLICATION_FAILED)
        m_appName = id;
        m_appName.truncate(4);//limit size of string to 4 chars, dlt cannot use more
        m_appDesc = description;

        s_fileContext = new DltContext;
        status = dlt_register_context(static_cast<DltContext*>(s_fileContext),"FILE","File Transfer Context");
        DLT_CHECK_ERROR(DLT_ERROR_REGISTER_CONTEXT_FAILED)
    }

    void DltAdapter::unregisterApplication()
    {
        int32_t status = 0;

        if(!m_dltInitialized)
        {
            return;
        }

        status = dlt_unregister_context(static_cast<DltContext*>(s_fileContext));
        DLT_CHECK_ERROR(DLT_ERROR_UNREGISTER_CONTEXT_FAILED)
        status = dlt_unregister_app();
        DLT_CHECK_ERROR(DLT_ERROR_UNREGISTER_APPLICATION_FAILED)
        delete static_cast<DltContext*>(s_fileContext);

        m_appName.truncate(0);
        m_appDesc.truncate(0);
    }

    void* DltAdapter::registerContext(capu::LogContext* ctx,const capu::String& id)
    {
        int32_t status = 0;

        if(!m_dltInitialized)
        {
            return (void*)0;
        }

        //if application name not set elsewhere use standard values so context can be created right here
        if(m_appName.getLength()==0)
        {
            registerApplication("ADEF","Default Application");
        }

        DltContext* dltContext = static_cast<DltContext*>(getDltContext(ctx).getPtr());
        if(dltContext)
        {
            return (void*)dltContext;//if pointer is set context is already initialized
        }

        dltContext = new DltContext;
        String contextName = ctx->getContextName();
        dlt_register_context(dltContext,id.c_str(),contextName.c_str());
        DLT_CHECK_ERROR(DLT_ERROR_REGISTER_CONTEXT_FAILED)

        ctx->setUserData(dltContext);

        return dltContext;
    }

    void DltAdapter::unregisterContext(capu::LogContext* ctx)
    {
        int32_t status = 0;

        if(!m_dltInitialized)
        {
            return;
        }

        DltContext* dltContext = static_cast<DltContext*>(getDltContext(ctx).getPtr());

        if(dltContext)
        {
            ctx->setUserData((void*)0);

            dlt_unregister_context(dltContext);
            DLT_CHECK_ERROR(DLT_ERROR_UNREGISTER_CONTEXT_FAILED)
            delete dltContext;
        }
    }

    void DltAdapter::registerInjectionCallback(capu::LogContext* ctx,uint32_t sid,int (*dlt_injection_callback)(uint32_t service_id, void *data, uint32_t length))
    {
        int32_t status = 0;

        if(!m_dltInitialized)
        {
            return;
        }

        DltContext* dltContext = static_cast<DltContext*>(getDltContext(ctx).getPtr());
        if(dltContext)
        {
            dlt_register_injection_callback(dltContext,sid,dlt_injection_callback);
            DLT_CHECK_ERROR(DLT_ERROR_INJECTION_CALLBACK_FAILURE)
        }
    }

    void DltAdapter::transmitFile(const String& uri, int delFile, int timeout)
    {
        int32_t status=0;
        
        if(!m_dltInitialized || uri.getLength() == 0 )
        {
            return;
        }
        dlt_user_log_file_infoAbout(static_cast<DltContext*>(s_fileContext),const_cast<const char*>(uri.c_str()));
        status = dlt_user_log_file_complete(static_cast<DltContext*>(s_fileContext),const_cast<const char*>(uri.c_str()),delFile,timeout);
        DLT_CHECK_ERROR(DLT_ERROR_FILETRANSFER_FAILURE)
    }

    DltContextPointer DltAdapter::getDltContext(const capu::LogContext* context)
    {
        return DltContextPointer(context->getUserData());
    }
}
