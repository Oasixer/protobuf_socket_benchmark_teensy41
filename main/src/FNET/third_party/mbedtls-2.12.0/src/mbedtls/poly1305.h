/**
 * \file poly1305.h
 *
 * \brief   This file contains Poly1305 definitions and functions.
 *
 *          Poly1305 is a one-time message authenticator that can be used to
 *          authenticate messages. Poly1305-AES was created by Daniel
 *          Bernstein https://cr.yp.to/mac/poly1305-20050329.pdf The generic
 *          Poly1305 algorithm (not tied to AES) was also standardized in RFC
 *          7539.
 *
 * \author Daniel King <damaki.gh@gmail.com>
 */

/*  Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved.
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of Mbed TLS (https://tls.mbed.org)
 */

#ifndef MBEDTLS_POLY1305_H
#define MBEDTLS_POLY1305_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "./config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include <stdint.h>
#include <stddef.h>

#define MBEDTLS_ERR_POLY1305_BAD_INPUT_DATA         -0x0057 /**< Invalid input parameter(s). */
#define MBEDTLS_ERR_POLY1305_FEATURE_UNAVAILABLE    -0x0059 /**< Feature not available. For example, s part of the API is not implemented. */
#define MBEDTLS_ERR_POLY1305_HW_ACCEL_FAILED        -0x005B  /**< Poly1305 hardware accelerator failed. */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(MBEDTLS_POLY1305_ALT)

typedef struct
{
    uint32_t r[4];      /** The value for 'r' (low 128 bits of the key). */
    uint32_t s[4];      /** The value for 's' (high 128 bits of the key). */
    uint32_t acc[5];    /** The accumulator number. */
    uint8_t queue[16];  /** The current partial block of data. */
    size_t queue_len;   /** The number of bytes stored in 'queue'. */
}
mbedtls_poly1305_context;

#else  /* MBEDTLS_POLY1305_ALT */
#include "poly1305_alt.h"
#endif /* MBEDTLS_POLY1305_ALT */

/**
 * \brief           This function initializes the specified Poly1305 context.
 *
 *                  It must be the first API called before using
 *                  the context.
 *
 *                  It is usually followed by a call to
 *                  \c mbedtls_poly1305_starts(), then one or more calls to
 *                  \c mbedtls_poly1305_update(), then one call to
 *                  \c mbedtls_poly1305_finish(), then finally
 *                  \c mbedtls_poly1305_free().
 *
 * \param ctx       The Poly1305 context to initialize.
 */
MBEDTLSFLASHMEM void mbedtls_poly1305_init( mbedtls_poly1305_context *ctx );

/**
 * \brief           This function releases and clears the specified Poly1305 context.
 *
 * \param ctx       The Poly1305 context to clear.
 */
MBEDTLSFLASHMEM void mbedtls_poly1305_free( mbedtls_poly1305_context *ctx );

/**
 * \brief           This function sets the one-time authentication key.
 *
 * \warning         The key must be unique and unpredictable for each
 *                  invocation of Poly1305.
 *
 * \param ctx       The Poly1305 context to which the key should be bound.
 * \param key       The buffer containing the 256-bit key.
 *
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_POLY1305_BAD_INPUT_DATA
 *                  if ctx or key are NULL.
 */
MBEDTLSFLASHMEM int mbedtls_poly1305_starts( mbedtls_poly1305_context *ctx,
                             const unsigned char key[32] );

/**
 * \brief           This functions feeds an input buffer into an ongoing
 *                  Poly1305 computation.
 *
 *                  It is called between \c mbedtls_cipher_poly1305_starts() and
 *                  \c mbedtls_cipher_poly1305_finish().
 *                  It can be called repeatedly to process a stream of data.
 *
 * \param ctx       The Poly1305 context to use for the Poly1305 operation.
 * \param ilen      The length of the input data (in bytes). Any value is accepted.
 * \param input     The buffer holding the input data.
 *                  This pointer can be NULL if ilen == 0.
 *
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_POLY1305_BAD_INPUT_DATA
 *                  if ctx or input are NULL.
 */
MBEDTLSFLASHMEM int mbedtls_poly1305_update( mbedtls_poly1305_context *ctx,
                             const unsigned char *input,
                             size_t ilen );

/**
 * \brief           This function generates the Poly1305 Message
 *                  Authentication Code (MAC).
 *
 * \param ctx       The Poly1305 context to use for the Poly1305 operation.
 * \param mac       The buffer to where the MAC is written. Must be big enough
 *                  to hold the 16-byte MAC.
 *
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_POLY1305_BAD_INPUT_DATA
 *                  if ctx or mac are NULL.
 */
MBEDTLSFLASHMEM int mbedtls_poly1305_finish( mbedtls_poly1305_context *ctx,
                             unsigned char mac[16] );

/**
 * \brief           This function calculates the Poly1305 MAC of the input
 *                  buffer with the provided key.
 *
 * \warning         The key must be unique and unpredictable for each
 *                  invocation of Poly1305.
 *
 * \param key       The buffer containing the 256-bit key.
 * \param ilen      The length of the input data (in bytes). Any value is accepted.
 * \param input     The buffer holding the input data.
 *                  This pointer can be NULL if ilen == 0.
 * \param mac       The buffer to where the MAC is written. Must be big enough
 *                  to hold the 16-byte MAC.
 *
 * \return          \c 0 on success.
 * \return          #MBEDTLS_ERR_POLY1305_BAD_INPUT_DATA
 *                  if key, input, or mac are NULL.
 */
MBEDTLSFLASHMEM int mbedtls_poly1305_mac( const unsigned char key[32],
                          const unsigned char *input,
                          size_t ilen,
                          unsigned char mac[16] );

#if defined(MBEDTLS_SELF_TEST)
/**
 * \brief           The Poly1305 checkup routine.
 *
 * \return          \c 0 on success.
 * \return          \c 1 on failure.
 */
MBEDTLSFLASHMEM int mbedtls_poly1305_self_test( int verbose );
#endif /* MBEDTLS_SELF_TEST */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_POLY1305_H */
