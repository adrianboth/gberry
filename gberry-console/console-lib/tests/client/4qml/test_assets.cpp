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

#include "client/4qml/assets.h"


TEST(Assets, BasicCases)
{
    Assets assets;
    assets.setRootPath("/foo/bar");

    ASSERT_TRUE(assets.filePath("123") == "file:/foo/bar/123");

    ASSERT_TRUE(assets.filePath("file:123") == "file:/foo/bar/123") << assets.filePath("file:123");

    ASSERT_TRUE(assets.filePath("/test/123") == "file:/test/123");

    ASSERT_TRUE(assets.filePath("file:/test/123") == "file:/test/123");
}

