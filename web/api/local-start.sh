#!/bin/bash

npm install

while ! (echo > /dev/tcp/pg/5432) > /dev/null 2>&1; do
  sleep 1
done

npx prisma migrate deploy

echo hello

npm run start