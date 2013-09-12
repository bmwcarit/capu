/*
 * Copyright (C) 2012 BMW Car IT GmbH
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

#ifndef CAPU_IPHONEOS_ARMV7_ATOMICOPERATION_H
#define CAPU_IPHONEOS_ARMV7_ATOMICOPERATION_H


namespace capu
{
    namespace os
    {
        namespace arch
        {
            class AtomicOperation
            {
            public:
                static uint32_t AtomicAdd32(volatile uint32_t& mem, uint32_t summand);
                static uint32_t AtomicSub32(volatile uint32_t& mem, uint32_t subtrahend);
                static uint32_t AtomicInc32(volatile uint32_t& mem);
                static uint32_t AtomicDec32(volatile uint32_t& mem);
            };

            inline
            uint32_t
            AtomicOperation::AtomicAdd32(volatile uint32_t& mem, uint32_t summand)
            {
                volatile uint32_t oldValue = 0;

                __asm__ volatile(
                    "1:                 \n"   //label
                    "ldrex %0, [%1]     \n"   //load mem into %0 == oldValue
                    "add   r1, %0, %2   \n"   //adds summand to mem and stores result in r1
                    "strex r0, r1, [%1] \n"   //store exclusive access result into r0
                    "cmp   r0, #0       \n"   //check if we have had exclusive access (r0 == 0)
                    "bne   1b"                //if we haved had no exclusive access, go to 1 and try it again
                    : "=&r"(oldValue)         //output
                    : "r"(&mem), "r"(summand) //input
                    : "r0", "r1"              //clobbered registers
                );

                return oldValue;
            }

            inline
            uint32_t
            AtomicOperation::AtomicSub32(volatile uint32_t& mem, uint32_t subtrahend)
            {
                volatile uint32_t oldValue = 0;

                __asm__ volatile(
                    "2:                    \n"   //label
                    "ldrex  %0, [%1]       \n"   //load mem into %0 == oldValue
                    "sub    r1,  %0,  %2   \n"   //subs subthrahend from mem and store result in r1
                    "strex  r0,  r1, [%1]  \n"   //store exclusive access result into r0
                    "cmp    r0,  #0        \n"   //check if we have had exclusive access
                    "bne    2b"                  //if there was no exclusive access, try it again
                    : "=&r"(oldValue)            //output
                    : "r"(&mem), "r"(subtrahend) //input
                    : "r0"  , "r1"               //clobbered registers
                );

                return oldValue;
            }

            inline
            uint32_t
            AtomicOperation::AtomicInc32(volatile uint32_t& mem)
            {
                return AtomicAdd32(mem, 1);
            }

            inline
            uint32_t
            AtomicOperation::AtomicDec32(volatile uint32_t& mem)
            {
                return AtomicSub32(mem, 1);
            }
        }
    }
}
#endif
