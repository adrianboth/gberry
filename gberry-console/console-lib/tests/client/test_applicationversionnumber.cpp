/* This file is part of GBerry.
 *
 * Copyright 2015 Tero Vuorela
 *
 * GBerry is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * GBerry is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBerry. If not, see <http://www.gnu.org/licenses/>.
 */

#include <testutils/qtgtest.h>

#include "client/applicationversionnumber.h"
using namespace GBerryLib;

TEST(ApplicationVersionNumber, BasicUseCase)
{
    ASSERT_FALSE(ApplicationVersionNumber::versionIsGreater("1", "1"));

    ASSERT_TRUE(ApplicationVersionNumber::versionIsGreater("2", "1"));
    ASSERT_FALSE(ApplicationVersionNumber::versionIsGreater("1", "2"));

    ASSERT_TRUE(ApplicationVersionNumber::versionIsGreater("1.1", "1"));
    ASSERT_TRUE(ApplicationVersionNumber::versionIsGreater("1.2", "1.1"));
    ASSERT_FALSE(ApplicationVersionNumber::versionIsGreater("1.1", "1.2"));

    ASSERT_TRUE(ApplicationVersionNumber::versionIsGreater("1.1.1", "1"));
    ASSERT_TRUE(ApplicationVersionNumber::versionIsGreater("1.1.1", "1.0.0"));
    ASSERT_TRUE(ApplicationVersionNumber::versionIsGreater("1.1.1", "1.0.9"));
    ASSERT_TRUE(ApplicationVersionNumber::versionIsGreater("1.1.1", "1.1.0"));
}

